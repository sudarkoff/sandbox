#!/usr/bin/perl -w
# 
#  Enforced originality!  If someone repeats something that has been already
#  said in channel, silence them.  Silence time increasing geometrically.
#  
#  Copyright (C) 2007  Dan Boger - zigdon+bot@gmail.com
#  
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#  
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software Foundation,
#  Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#

use strict;
use Net::IRC;
use Time::HiRes qw/usleep/;
use DBI;
use Date::Calc qw/Normalize_DHMS/;
use Data::Dumper;

use constant { DEBUG => 0 };

# IRC info
my $irc_server       = "irc.example.com";
my $irc_chan         = "#xkcd-signal";
my $irc_nick         = "botname";
my $irc_pass         = "botpass";
my $irc_name         = "I AM MODERATOR";
my $irc_nick_default = $irc_nick;

# Database info
my $db_name          = "schema_name";
my $db_user          = "database_user";
my $db_pass          = "sekret";

# Other settings
my $home    = "/path/to/robot/dir";
my $logfile = "$home/robot9000.log";
my $common_file      = "$home/words.lst";
my $timeout_limit    = 365*24*60*60;  # 1 year == kick/ban

my $next_unban = 1;
my $maint_time = time + 20;

# Authorized users
my %auth = (
    zigdon => 'user@hostmask.example.com',
    xkcd   => 'other@hostmask2.example.com',
);

my $topic;
my %nicks;
my %nicks_tmp;
my $nick_re;
my %nick_changes;
my %common_words;
my %sql;

my ( $irc, $irc_conn, $dbh ) = &setup;

$irc_conn->names;
&event_loop;

sub logmsg {
    print scalar localtime, " - @_\n";
    print LOG scalar localtime, " - @_\n",;
}

sub event_loop {

    #warn "event_loop (@_)\n";
    while (1) {
        $irc->do_one_loop();
        usleep 50;

        if ( $next_unban and time > $next_unban ) {
            &process_unbans;
        }

        if ( time > $maint_time ) {
            logmsg "Running maint";

            foreach ( keys %nick_changes ) {
                next if $nick_changes{$_}[0] + 300 > time;

                logmsg "Clearing nick_changes for $_";
                delete $nick_changes{$_};
            }

            if (0) {

                #$irc_conn->debug(1);
                $irc_conn->privmsg( 'nickserv', "status " . join " ",
                    keys %nicks );

                $sql{high_score}->execute;
                my ( $hi_nick, $hi_score ) = $sql{high_score}->fetchrow_array;
                $sql{high_score}->finish;
                logmsg "High score: $hi_nick, $hi_score";

                my $oldtopic = $topic;
                $topic =~
s/High score:(?: \S+ \S+)? \|/High score: $hi_nick $hi_score |/;
                if ( $topic ne $oldtopic ) {
                    logmsg "Setting topic to $topic";
                    $irc_conn->topic( $irc_chan, $topic );
                }
            }

            $maint_time = time + 300;
        }
    }
}

sub process_unbans {
    $sql{get_unbans}->execute(time);
    while ( my ( $nick, $userhost, $id, $bantype ) =
        $sql{get_unbans}->fetchrow_array )
    {
        logmsg "Restoring $userhost";
        if ( $bantype eq 'v' ) {
            $irc_conn->mode( $irc_chan, "+v", $nick );
        }
        else {
            $irc_conn->mode( $irc_chan, "+v", $nick );
            $irc_conn->mode( $irc_chan, "+o", $nick );
        }
        $sql{clear_ban}->execute($id);

        #$irc_conn->privmsg( $nick, "you may now speak in $irc_chan." );
    }

    $sql{next_unban}->execute;
    ($next_unban) = $sql{next_unban}->fetchrow_array;
    $sql{next_unban}->finish;
}

sub setup {

    #warn "setup (@_)\n";
    # open our log file
    open LOG, ">>$logfile" or die "Can't write to $logfile: $!";
    logmsg "Starting up";
    sleep 5;

    # log into IRC
    logmsg "Connecting irc://$irc_nick";
    $irc = new Net::IRC;
    my $irc_conn = $irc->newconn(
        Nick     => $irc_nick,
        Server   => $irc_server,
        Ircname  => $irc_name,
    );

    if (DEBUG) {
        open DEBUG_FH, ">>$logfile.debug"
          or die "Can't write to $logfile.debug: $!";
        $irc_conn->add_default_handler( \&dump_event );
    }

    # talk events
    $irc_conn->add_handler( public  => \&irc_on_public );
    $irc_conn->add_handler( caction => \&irc_on_public );
    $irc_conn->add_handler( notice  => \&irc_on_notice );
    $irc_conn->add_handler( msg     => \&irc_on_msg );

    # user events
    $irc_conn->add_handler( nick => \&irc_on_nick );
    $irc_conn->add_handler( join => \&irc_on_joinpart );
    $irc_conn->add_handler( part => \&irc_on_joinpart );
    $irc_conn->add_handler( quit => \&irc_on_joinpart );

    # server events
    $irc_conn->add_handler( 376        => \&irc_on_connect );    # end of MOTD
    $irc_conn->add_handler( topic      => \&irc_on_topic );
    $irc_conn->add_handler( namreply   => \&irc_on_names );
    $irc_conn->add_handler( endofnames => \&irc_on_endnames );
    $irc_conn->add_handler(
        chanoprivsneeded => sub {
            logmsg "Reauthing to nickserv";
            $irc_conn->privmsg( "nickserv", "identify $irc_pass" );
        }
    );

    # connect to the database
    logmsg "Connecting dbi://$db_user\@$db_name";
    my $dbh = DBI->connect( "DBI:mysql:database=$db_name", $db_user, $db_pass )
      or die "Can't connect to the database!";

    logmsg "Preparing SQL statements";
    $sql{lookup_line} = $dbh->prepare(
        "select id from `lines`
                    where msg = ?
                    limit 1"
    );
    $sql{add_line} = $dbh->prepare(
        "insert into `lines` (msg)
                  values (?)"
    );
    $sql{lookup_user} = $dbh->prepare(
        "select timeout_power, banned_until from users
                    where mask = ?
                    limit 1"
    );
    $sql{lookup_mask} = $dbh->prepare(
        "select mask 
                      from users
                    where nick = ?
                    order by last_talk desc
                    limit 1"
    );
    $sql{update_user} = $dbh->prepare(
        "update users
                        set timeout_power = timeout_power + 2,
                                banned_until = ?,
                                nick = ?, 
                                total_bans = total_bans + 1,
                                ban_type = ?
                    where mask = ?
                    limit 1"
    );
    $sql{add_user} = $dbh->prepare(
"insert into users (banned_until, nick, mask, timeout_power, lines_talked, total_bans, ban_type)
                  values (?, ?, ?, ?, 0, 0, ?)"
    );
    $sql{user_talk} = $dbh->prepare(
        "update users
                        set lines_talked = lines_talked + 1,
                                word_count   = word_count + ? + 1,
                                last_talk    = null
                    where mask = ?
                    limit 1"
    );
    $sql{next_unban} = $dbh->prepare(
        "select min(banned_until)
                      from users
                    where banned_until > 0"
    );
    $sql{get_unbans} = $dbh->prepare(
        "select nick, mask, id, ban_type
                      from users
                    where banned_until > 0
                        and banned_until <= ?"
    );
    $sql{clear_ban} = $dbh->prepare(
        "update users
                        set banned_until = 0
                    where id = ?"
    );
    $sql{high_score} = $dbh->prepare(
"select nick, lines_talked/word_count * lines_talked/(total_bans + 1) as score
                  from users
                  order by lines_talked/word_count * lines_talked/(total_bans + 1) desc, lines_talked desc
                  limit 1"
    );

    logmsg "Setup complete";

    logmsg "Loading common words...";
    open( WORDS, $common_file ) or die "Can't read $common_file: $!";
    while (<WORDS>) {
        chomp;
        $common_words{ lc $_ } = 1;
    }
    close WORDS;
    logmsg "Loaded ", scalar keys %common_words, " words";

    return ( $irc, $irc_conn, $dbh );
}

# event handlers
sub irc_on_connect {

    #warn "irc_on_connect (@_)\n";
    my ( $self, $event ) = @_;

    logmsg "Connected to IRC, joining $irc_chan";
    $self->join($irc_chan);

    logmsg "Authenticating";
    $self->privmsg( "nickserv", "identify $irc_pass" );
}

sub irc_on_notice {
    my ( $self, $event ) = @_;
    my ( $nick, $msg ) = ( $event->nick, $event->args );

    logmsg "Notice from $nick to " . @{ $event->to }[0] . ": $msg";
    return if ${ $event->to }[0] ne $irc_chan;

    &fail( $self, $nick, $event->userhost );
}

sub irc_on_msg {

    #warn "irc_on_msg (@_)\n";
    my ( $self, $event ) = @_;
    my ( $nick, $msg ) = ( $event->nick, $event->args );
    my @args;
    ( $msg, @args ) = split ' ', $msg;

    logmsg "PRIVMSG $nick($nicks{$nick}): $msg @args";
    if ( lc $msg eq 'timeout' ) {
        my ( $timeout, $banned_until );
        if ( $args[0] ) {
            if ( $sql{lookup_mask}->execute( $args[0] ) > 0 ) {
                my ($mask) = $sql{lookup_mask}->fetchrow_array;
                $sql{lookup_mask}->finish;
                ( $timeout, $banned_until ) = &get_timeout($mask);
            }
        }
        else {
            ( $timeout, $banned_until ) = &get_timeout( $event->userhost );
        }

        if ($timeout) {
            $timeout = &timeout_to_text( 2**( $timeout + 2 ) );

            $self->privmsg( $nick, "Next timeout will be $timeout" );

            if ($banned_until) {
                $self->privmsg( $nick,
                    "Currently muted, can speak again in "
                      . &timeout_to_text($banned_until - time) );
            }
        }
        else {
            $self->privmsg( $nick, "No timeout found" );
        }
    }
    elsif (
        ( exists $auth{ lc $nick } and $event->userhost eq $auth{ lc $nick } )
        or $nicks{$nick} =~ /[@&%]/ )
    {
        logmsg "AUTH $nick: $msg ($nicks{$nick})";
        if ( $msg eq 'quit' and exists $auth{ lc $nick } ) {
            $self->privmsg( $nick, "Quitting" );
            exit;
        }
        elsif ( $msg eq 'msg' and exists $auth{ lc $nick } ) {
            $self->privmsg( $nick, "Ok - sending $args[0]: @args[1..$#args]" );
            $self->privmsg( $args[0], join " ", @args[ 1 .. $#args ] );
            logmsg "Sending MSG to $args[0]: @args[1..$#args]";
        }
        elsif ( $msg eq 'unban' ) {
            logmsg "Unbanning $args[0] by command";
            $self->mode( $irc_chan, "-b", $args[0] );
        }
        elsif ( $msg eq 'kick' ) {
            logmsg "Kicking $args[0] by command";
            $self->kick(
                $irc_chan, $args[0],
                $args[1]
                ? join " ",
                $args[ 1 .. $#args ]
                : "Kick"
            );
        }
        elsif ( $msg eq 'fail' and $args[0] =~ /([^!]+)!(\S+)/ ) {
            logmsg "Failing $1!$2 by command";
            &fail( $self, $1, $2 );
        }
        elsif ( $msg eq 'nick_re' ) {
            logmsg "Current nick re: $nick_re";
            $self->privmsg( $nick, "Ok, logged" );
        }
        elsif ( $msg eq 'fail' ) {
            if ( $sql{lookup_mask}->execute( $args[0] ) > 0 ) {
                my ($mask) = $sql{lookup_mask}->fetchrow_array;
                $sql{lookup_mask}->finish;
                logmsg "Failing $args[0]!$mask by command";
                &fail( $self, $args[0], $mask );
            }
            else {
                logmsg "Couldn't find mask for $args[0]";
            }
        }
        else {
            foreach (
"Commands: timeout - query if you're banned, and what your next ban will be",
                "          timeout <nick> - same, for someone else",
                "          unban   - unban given nickmask",
                "          kick <nick> <msg> - kick someone",
"          fail <nick> - have the moderator manually silence <nick>"
              )
            {
                $self->privmsg( $nick, $_ );
            }
        }
    }
    else {
        $self->privmsg( $nick,
"Commands: timeout <nick> - query if you're banned, and what your next ban will be"
        );
        logmsg "Ignoring PRIVMSG from $nick";
    }

}

# public msg - someone talking in chat
sub irc_on_public {

    #warn "irc_on_public (@_)\n";
    my ( $self, $event ) = @_;
    my ( $nick, $userhost ) = ( $event->nick, $event->userhost );
    my ($msg) = ( $event->args );

    logmsg "$nick: $msg";

    # process the message so that we strip them down

    # remove case
    $msg = lc $msg;

    # remove addressing nicks:
    $msg =~ s/^\S+: ?//;

    # remove any nicks referred to
    $msg =~ s/\b(?:$nick_re)\b/ /g;

    # remove punct
    $msg =~ s/[^a-zA-Z\d -]+//g;

    # removing leading/trailing/multiple spaces
    $msg =~ s/^\s+|\s+$//g;
    $msg =~ s/\s\s+/ /g;

    # check if the line was already in the DB
    my $res = $sql{lookup_line}->execute($msg);

    if ( $res > 0 ) {

        # kick!
        &fail( $self, $nick, $userhost );
    }
    else {

        # add it as a new line
        $sql{add_line}->execute($msg);

        my $words = ( $msg =~ tr/ / / );
        if ( $sql{user_talk}->execute( $words, $userhost ) == 0 ) {
            $sql{add_user}->execute( 0, $nick, $userhost, 1, "v" );
            $sql{user_talk}->execute( $words, $userhost );
        }
    }

    $sql{lookup_line}->finish;
}

sub get_timeout {
    my $mask = shift;

    $sql{lookup_user}->execute($mask);
    my ( $timeout, $banned_until ) = $sql{lookup_user}->fetchrow_array;
    $sql{lookup_user}->finish;

    return ( $timeout, $banned_until );
}

sub timeout_to_text {
    my $timeout = shift;

    my ( $dd, $dh, $dm, $ds ) = Normalize_DHMS( 0, 0, 0, $timeout );
    my $delta_text;
    $delta_text .= "$dd day" .    ( $dd == 1 ? " " : "s " ) if $dd;
    $delta_text .= "$dh hour" .   ( $dh == 1 ? " " : "s " ) if $dh;
    $delta_text .= "$dm minute" . ( $dm == 1 ? " " : "s " ) if $dm;
    $delta_text .= "$ds second" . ( $ds == 1 ? " " : "s " ) if $ds;
    $delta_text =~ s/ $//;

    return $delta_text;
}

# fail - silence for 2**2n
sub fail {

    my ( $self, $nick, $userhost ) = @_;

    logmsg "Failing $nick ($userhost)";

    # look up the last timeout value for this userhost, default is 1
    my ($timeout) = &get_timeout($userhost);

    $timeout += 2;

    if (2**$timeout > $timeout_limit) {
      logmsg "Kickbanning $nick ($userhost)";
      $self->notice( $irc_chan, "$nick, thanks for playing!" );
      $self->mode( $irc_chan, "+b", $userhost );
      $self->kick( $irc_chan, $nick, "Go away" );
      return;
    }

    my $delta_text = &timeout_to_text( 2**$timeout );

    $self->notice( $irc_chan, "$nick, you have been muted for $delta_text." );

    my $bantype = "v";
    if ( not $nicks{$nick} or $nicks{$nick} eq '+' or $nicks{$nick} eq '1' ) {
        $self->mode( $irc_chan, "-v", $nick );
    }
    else {
        logmsg
          "$nick is an operator ($nicks{$nick}) - deopping first";
        $self->mode( $irc_chan, "-o", $nick );
        $self->mode( $irc_chan, "-v", $nick );
        $bantype = "b";
    }

    my $target = time + 2**$timeout;

    if (
        $sql{update_user}->execute( $target, $nick, $bantype, $userhost ) == 0 )
    {
        $sql{add_user}->execute( $target, $nick, $userhost, 2, $bantype );
    }
    logmsg "Silenced for " . ( 2**$timeout ) . " seconds";

    if ( not $next_unban or $target < $next_unban ) {
        $next_unban = $target;
    }
}

sub kick {

    #warn "kick (@_)\n";
    my ( $self, $nick, $userhost, $msg ) = @_;

    &fail( $self, $nick, $userhost );

    $msg ||= "Go away";

    logmsg "Kicking $nick ($userhost): $msg";

    $self->kick( $irc_chan, $nick, $msg );
}

sub load_log {
    while (<>) {

        # http://isomerica.net/~xkcd/#xkcd.log
        # 20:50 <@zigdon> oh, right, he can't actually kick you
        # 20:56  * zigdon tests
        #

        next unless /[>*]/;
        chomp;
        s/^[^>]+> //
          or s/^[^*]+\* \S+ //;

        s/^\s+|\s+$//g;
        s/\s\s+/ /g;

        # remove addressing nicks:
        s/^\S+: ?//;

        # remove punct
        s/[^a-zA-Z\d -]+//g;

        # remove case
        my $msg = lc $_;

        my $res = $sql{lookup_line}->execute($msg);
        next if $res > 0;
        print "$msg\n";
        $sql{add_line}->execute($msg);
    }
    exit;
}

sub update_nick_re {
    $nick_re = $irc_nick;
    $nick_re .= "|\Q$_\E"
      foreach grep { not exists $common_words{$_} } keys %nicks;
    $nick_re = qr/$nick_re/;

    #logmsg "Nick_re = $nick_re";
}

sub irc_on_nick {
    my ( $self, $event ) = @_;
    my ( $oldnick, $newnick, $userhost ) =
      ( lc $event->nick, $event->args, $event->userhost );
    $newnick = lc $newnick;

    $nicks{$newnick} = $nicks{$oldnick};
    delete $nicks{$oldnick};

# if someone changes nicks too often (more than 3 times in a maint period), that's a fail
    if ( exists $nick_changes{$userhost} ) {
        if ( $nick_changes{$userhost}[1]++ > 2 ) {
            &fail( $self, $newnick, $userhost );
        }
        elsif ( $nick_changes{$userhost}[1] > 5 ) {
            &kick( $self, $newnick );
        }
    }
    else {
        $nick_changes{$userhost} = [ time, 1 ];
    }

    logmsg
      "$oldnick is now known as $newnick ($nick_changes{$userhost}[1] since ",
      scalar localtime $nick_changes{$userhost}[0], ")";
    &update_nick_re;
}

sub irc_on_joinpart {
    my ( $self, $event ) = @_;
    my ($nick) = lc $event->nick;

    my $action;
    if ( $event->{type} eq 'join' ) {
        $nicks{$nick} = 1;
        $action = "joined";

        # if this is a new user, give them voice after a minute (disabled)
        # if it's an existing user, and they're not currently banned, give them
        # voice immediately
        if ( $sql{lookup_user}->execute( $event->userhost ) > 0 or 1 ) {
            my ( $power, $ban ) = $sql{lookup_user}->fetchrow_array;
            $sql{lookup_user}->finish;
            unless ($ban) {
                $irc_conn->mode( $irc_chan, "+v", $nick );
            }
        }
        else {
            $sql{add_user}
              ->execute( time + 60, $nick, $event->userhost, 1, "v" );
            if ( not $next_unban or time + 60 < $next_unban ) {
                $next_unban = time + 60;
            }
            $irc_conn->privmsg( $nick,
"Welcome to #xkcd-signal.  Give me a minute, and I'll set you up with a voice.  In the meantime, you might want to read the FAQ: http://www.xkcdb.com/signalfaq"
            );
        }
    }
    else {
        delete $nicks{$nick};
        $action = "left";
    }
    logmsg "$nick has $action the channel";
    &update_nick_re;
}

sub irc_on_names {
    my ( $self, $event ) = @_;
    my ( $nick, $mynick ) = ( $event->nick, $self->nick );
    my ($names) = ( $event->args )[3];

    print "Event: $_[1]->{type}\n";
    print DEBUG_FH Dumper [ @_[ 1 .. $#_ ] ] if DEBUG;

    %nicks_tmp =
      ( %nicks_tmp, map { s/^(\W)//; ( $_ => $1 ? $1 : 1 ) } split ' ', $names );
    logmsg "Got more names - current total: ", scalar keys %nicks_tmp;
}

sub irc_on_endnames {
    my ( $self, $event ) = @_;

    print "Event: $_[1]->{type}\n";
    print DEBUG_FH Dumper [ @_[ 1 .. $#_ ] ] if DEBUG;

    %nicks = (%nicks_tmp);
    %nicks_tmp = ();
    &update_nick_re;

    logmsg "Names done - in channel: ", join ", ", sort keys %nicks;
}

sub irc_on_topic {
    my ( $self, $event ) = @_;

    $topic = ( $event->args )[2];
    logmsg "Topic updated to '$topic'";
}

sub dump_event {
    print "Event: $_[1]->{type}\n";
    print DEBUG_FH Dumper [ @_[ 1 .. $#_ ] ];
}

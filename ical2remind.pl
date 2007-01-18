#!/usr/bin/perl -w
#
#Copyright (c) 2005, Justin B. Alcorn
#All rights reserved.
#
#Redistribution and use in source and binary forms, with or without
#modification, are permitted provided that the following conditions are met:
#
#1) Redistributions of source code must retain the above copyright notice,
#   this list of conditions and the following disclaimer.
#
#2) Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
#3) Neither the name of Justin B. Alcorn nor the names of his
#   contributors may be used to endorse or promote products derived from this
#   software without specific prior written permission.
#
#THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
#ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
#LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
#CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
#SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
#INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
#CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#POSSIBILITY OF SUCH DAMAGE.
#
# Reads iCal files and outputs remind-compatible files.   Tested ONLY with
#   calendar files created by Mozilla Calendar/Sunbird. Use at your own risk.
#
# version 0.1 - ALPHA CODE.  

use strict;
use iCal::Parser;
use Data::Dumper;
use Time::Local;
use DateTime;

my $cal_dir = "local/cal";
my $DEFAULT_LEAD_TIME = 3;
my $PROJECT_LEAD_TIME = 14;

opendir(DIR, $cal_dir) || die "can't opendir $cal_dir: $!";
my @cals = grep { /.ics$/ && -f "$cal_dir/$_" } readdir(DIR);
closedir DIR;

my $parser = iCal::Parser->new();
my $month = ['None','Jan','Feb','Mar','Apr','May','Jun','Jul','Aug','Sep','Oct','Nov','Dec'];
my $hash;
foreach my $cal (@cals) {
    $hash = $parser->parse($cal_dir."/".$cal);
}

my ($key, $todo, @newtodos, $leadtime);
print 'MSG Calendar ToDos:%"%"%'."\n";
my $todos = $hash->{'todos'};
# For sorting, make sure everything's got something
#   To sort on.  
foreach $todo (@{$todos}) {
	# remove completed items
	if ($todo->{'STATUS'} eq 'COMPLETED') {
		next;
	} elsif ($todo->{'DUE'}) {
		# All we need is a due date, everything else is sugar
		$todo->{'SORT'} = $todo->{'DUE'}->clone;
	} elsif ($todo->{'DTSTART'}) {
		# for sorting, sort on start date if there's no due date
		$todo->{'SORT'} = $todo->{'DTSTART'}->clone;
	} else {
		# if there's no due or start date, just make it now.
		$todo->{'SORT'} = DateTime->now;
	}
	push(@newtodos,$todo);
}
# Now sort on the new Due dates and print them out.  
foreach $todo (sort { DateTime->compare($a->{'SORT'}, $b->{'SORT'}) } @newtodos) {
	my $due = $todo->{'SORT'}->clone();
	if (defined($todo->{'DTSTART'}) && defined($todo->{'DUE'})) {
		# Lead time is duration of task + lead time
		my $diff = ($todo->{'DUE'}->delta_days($todo->{'DTSTART'})->days())+$DEFAULT_LEAD_TIME;
		$leadtime = "+".$diff;
	} else {
		$leadtime = "+".$DEFAULT_LEAD_TIME;
	}
	print "REM ".$due->month_abbr." ".$due->day." ".$due->year." $leadtime MSG \%a $todo->{'SUMMARY'}\%\"\%\"\%\n";
}

my ($yearkey, $monkey, $daykey,$uid,%eventsbyuid);
print 'REM MSG %"%"%'."\n";
print 'MSG Calendar Events:%"%"%'."\n";
my $events = $hash->{'events'};
foreach $yearkey (sort keys %{$events} ) {
	my $yearevents = $events->{$yearkey};
	foreach $monkey (sort {$a <=> $b} keys %{$yearevents}){
		my $monevents = $yearevents->{$monkey};
		foreach $daykey (sort {$a <=> $b} keys %{$monevents} ) {
			my $dayevents = $monevents->{$daykey};
			foreach $uid (sort {
							DateTime->compare($dayevents->{$a}->{'DTSTART'}, $dayevents->{$b}->{'DTSTART'})	
							} keys %{$dayevents}) {
				my $event = $dayevents->{$uid};
	           if ($eventsbyuid{$uid}) {
					my $curreventday = $event->{'DTSTART'}->clone;
					$curreventday->truncate( to => 'day' );
					$eventsbyuid{$uid}{$curreventday->epoch()} =1;
					for (my $i = 0;$i < 4 && !defined($event->{'LEADTIME'});$i++) {
						if ($eventsbyuid{$uid}{$curreventday->subtract( days => $i+1 )->epoch() }) {
	                    	$event->{'LEADTIME'} = $i;
						}
					}
                } else {
                    $eventsbyuid{$uid} = $event;
					my $curreventday = $event->{'DTSTART'}->clone;
					$curreventday->truncate( to => 'day' );
					$eventsbyuid{$uid}{$curreventday->epoch()} =1;
                }

			}
		}
	}
}
foreach $yearkey (sort keys %{$events} ) {
	my $yearevents = $events->{$yearkey};
	foreach $monkey (sort {$a <=> $b} keys %{$yearevents}){
		my $monevents = $yearevents->{$monkey};
		foreach $daykey (sort {$a <=> $b} keys %{$monevents} ) {
			my $dayevents = $monevents->{$daykey};
			foreach $uid (sort {
							DateTime->compare($dayevents->{$a}->{'DTSTART'}, $dayevents->{$b}->{'DTSTART'})
							} keys %{$dayevents}) {
				my $event = $dayevents->{$uid};
				if (exists($event->{'LEADTIME'})) {
					$leadtime = "+".$event->{'LEADTIME'};
				} elsif ($event->{'CATEGORIES'} && $event->{'CATEGORIES'} eq 'Projects') {
					$leadtime = "+".$PROJECT_LEAD_TIME;
				} else {
					$leadtime = "+".$DEFAULT_LEAD_TIME;
				}
				my $start = $event->{'DTSTART'};
				print "REM ".$start->month_abbr." ".$start->day." ".$start->year." $leadtime ";
				if ($start->hour > 0) { 
					print " AT ";
					print $start->strftime("%H:%M");
					print " MSG %a %2 ";
				} else {
					print " MSG %a ";
				}
				print "%\"$event->{'SUMMARY'}";
				print " at $event->{'LOCATION'}" if $event->{'LOCATION'};
				print "\%\"%\n";
			}
		}
	}
}
exit 0;

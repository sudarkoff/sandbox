#!/usr/bin/perl
# Time-stamp: "1998-09-21 21:00:49 MDT sburke@alf8.speech.cs.cmu.edu"
#
require 5;
$html = 1;
$dvorak =  lc($ARGV[0]) eq 'dvorak' ? 1 : 0;
#--------------------------------------------------------------------------

if ($dvorak) {
    $type = 'Dvorak';
    $l1 = 'py';     $r1 = 'fgcrl';
    $l2 = 'aoeui';  $r2 = 'dhtns';
    $l3 = 'qjkx';   $r3 = 'bmwvz';
} else {
    $type = 'QWERTY';
    $l1 = 'qwert';  $r1 = 'yuiop';
    $l2 = 'asdfg';  $r2 = 'hjkl';
    $l3 = 'zxcvb';  $r3 = 'nm';
}
$l = $l1 . $l2 . $l3;
$r = $r1 . $r2 . $r3;
#print $html ? "<H1>For The $type Keyboard</H1>\n"
#    : "For The $type Keyboard:\n";

# get down to biz
open(IN, '< /usr/share/dict/words') or die "no words found\n";

#filter_for('Top Row',          sub { /^[$r1$l1]+$/io } );
filter_for('Middle Row',       sub { /^[$r2$l2]+$/io } );
#filter_for('Bottom Row',       sub { /^[$r3$l3]+$/io } );

#filter_for('Top Row Left',     sub { /^[$l1]+$/io } );
#filter_for('Top Row Right',    sub { /^[$r1]+$/io } );
#filter_for('Middle Row Left',  sub { /^[$l2]+$/io } );
#filter_for('Middle Row Right', sub { /^[$r2]+$/io } );
#filter_for('Bottom Row Left',  sub { /^[$l3]+$/io } );
#filter_for('Bottom Row Right', sub { /^[$r3]+$/io } );

#filter_for('Left Side',        sub { /^[$l]+$/io } );
#filter_for('Right Side',       sub { /^[$r]+$/io } );

#filter_for('Alternating Left-Right-Left',
#           sub { /^([$r][$l])+[$r]?$/io || /^([$l][$r])+[$l]?$/io } );

exit;

sub filter_for {
    my($heading, $criterion) = @_[0,1];
    my $match_count = 0;

#    printf $html ? "\n<H2>%s</H2>\n<BLOCKQUOTE>" : "\n%s\n", $heading;
    
    seek(IN, 0, 0);
    while(<IN>) {
	if(/[aeiouy]/i and not /^[A-Z][A-Z]/ # ignore acronyms and abbrevs
	   and &$criterion) { print; ++$match_count; }
    }
#    printf
#	$html ? "</BLOCKQUOTE><B>total: %d words</B>\n" : "total: %d words\n",
#	$match_count;
}

__END__


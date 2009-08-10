#!/usr/bin/perl -w

$symbols = $ARGV[0];

while(<STDIN>) {
	$_ =~ s/(_Z.*?)\s/`c++filt $1`/eg;
	if(/.*?qtlobby.exe:([0-9A-Fa-f]+)/) {
		chop; chop;
		print;
		$output = `addr2line -e "$symbols" 0x$1`;
		if($output =~ /.*?(\w+\..*?:\d+)/) {
			print "  $1";
		}
		print "\n";
	} else {
		print;
	}
}


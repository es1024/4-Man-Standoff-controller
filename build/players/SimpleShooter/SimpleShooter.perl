#!/usr/bin/perl
@history = map([split(",",$_)],split(" ",$ARGV[1]));
$target = 1;
for(2..3){
 if($history[$_][0] >= $history[$target][0]){$target = $_}
}
print "S$target"

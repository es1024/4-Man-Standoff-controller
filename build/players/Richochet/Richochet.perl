#!/usr/bin/perl
@history = map([split(",",$_)],split(" ",$ARGV[1]));
$health = $history[0][0];
@options = ();
for(1..3){
 if($history[$_][0] > 0){
  push(@options,$_);
 }
}
$target = @options[int(rand(~~@options))];
if(~~@{$history[0]} == 50){
 $target = 0;
}
print"S$target";

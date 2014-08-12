#!/usr/bin/perl

@allinfo = map { [split/,/] } split / /, $ARGV[1];
@life = map { $_->[0] } @allinfo;
@action = map { @$_>1 ? $_->[-1] : () } @allinfo;

if($life[0] < 3 && rand() < .5 )
{
    printf "D%d", +(sort { ($life[$a]>0)*($action[$a] eq "N") <=> ($life[$b]>0)*($action[$b] eq "N") } 1..3)[2]
}
else
{
    @score = map { $life[$_]>0 ? (5/$life[$_] + 2*($action[$_] =~ /S./)) : 0 } 1..3;
    printf "S%d", +(sort { $score[$a] <=> $score[$b] } 1..3);
}

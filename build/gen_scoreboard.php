<?php
$dir = new DirectoryIterator(dirname(__FILE__) . '/results');
$points = array();
$bonus = array(4,2,1,0);
$num_runs = (35*34*33)/(3*2*1)*3;
$out = '';

foreach($dir as $f){
	if(!$f->isDot()){
		$a = explode("\n", file_get_contents(dirname(__FILE__) . '/results/' . $f->getFilename()));
		for($i = 0; $i < 3; ++$i){
			$r = array();
			for($j = 1; $j < 5; ++$j){
				$r[] = sscanf($a[$i*5+$j],"%30s (%d)");
			}
			$p = array($r[0][1],$r[1][1],$r[2][1],$r[3][1]);
			rsort($p);
			// see standoff.cpp for details
			for($j = 1; $j < 4; ++$j)
				if($p[$j] == $p[$j-1]) ++$p[$j-1];
			for($j = 0; $j < 4; ++$j)
				$points[$r[$j][0]] += 35 + $bonus[array_search($r[$j][1], $p)] + $r[$j][1];

		}
	}
}
arsort($points);
foreach($points as $k=>$p){
	$out .= "$k " . ($p / $num_runs) . PHP_EOL;
}

file_put_contents('results/scoreboard.log', $out);

system("echo Generated: `date \"+%Y/%m/%d %H:%M:%S.%N UTC\"` >> results/scoreboard.log"); 

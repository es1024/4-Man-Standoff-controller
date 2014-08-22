#include "pool.h"
#include "entry.h"
#include "standoff.h"

#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cassert>
#include <iomanip>
#include <unistd.h>

// minimum 4
// classpath: players/.
const char *entry_info[] = { 
	"Grenadier",					"java",		"Grenadier", 
	"Pacifist",						"",			"./players/Pacifist/Pacifist", 
	"StraightShooter",				"",			"./players/StraightShooter/StraightShooter.perl",
	"InputAnalyzer",				"",			"./players/InputAnalyzer/InputAnalyzer", 
	"PoliticallyCorrectGunman",		"python",	"./players/PoliticallyCorrectGunman/politicallycorrectgunman.py",
	"HanSolo",						"python2",	"./players/HanSolo/hansolo.py",
	"DONTNUKEMEBRO",				"lua",		"./players/DONTNUKEMEBRO/DONTNUKEMEBRO.lua",
	"Spock",						"python",	"./players/Spock/Spock.py",
	"SimpleShooter",				"",			"./players/SimpleShooter/SimpleShooter.perl",
	"Coward",						"",			"./players/Coward/Coward.perl",
	"Aggressor",					"",			"./players/Aggressor/Aggressor",
	"Richochet",					"",			"./players/Richochet/Richochet.perl",
	"Zaenille",						"",			"./players/Zaenille/Zaenille",
	"Monkey",						"python",	"./players/Monkey/monkey.py",
	"TwentyFourthsAndAHalfCentury",	"python",	"./players/TwentyFourthsAndAHalfCentury/TwentyFourthsAndAHalfCentury.py",
	"Bomberman",					"",	"./players/Bomberman/Bomberman.sh",
	"EmoCowboy",					"python",	"./players/EmoCowboy/EmoCowboy.py",
	"Ninja",						"lua",		"./players/Ninja/ninja.lua",
	"AntiGrenadier",				"lua",		"./players/AntiGrenadier/AntiGrenadier.lua",
	"Rule0Bot",						"python2",	"./players/Rule0Bot/rule0bot.py",
	"Sniper",						"lua",		"./players/Sniper/Sniper.lua",
	"Scared",						"python",	"./players/Scared/scared.py",
	"Neo",							"java",		"Neo",
	"CourageTheDog",				"python2",	"./players/CourageTheDog/CourageTheDog.py",
	"ManipulativeBastard",			"python2",	"./players/ManipulativeBastard/ManipulativeBastard.py",
	"Osama",						"",			"./players/Osama/osama",
	"Darwin",						"python",	"./players/Darwin/Darwin.py",
	"Equivocator",					"lua",			"./players/Equivocator/Equivocator.lua",
	"Observer",						"java",		"Observer",
	"BiasedOne",					"java",		"BiasedOne",
	"Hippolyta",					"",			"./players/Hippolyta/Hippolyta",
	"PriorityTargets",				"ruby",		"./players/PriorityTargets/PriorityTargets.rb",
	"MAD",							"java",		"MAD",
	"Sadist",					"python2",	"./players/Sadist/Sadist.py",
//	"SolidSnake",					"java",		"SolidSnake",
	"MuhammadAli",					"java",		"MuhammadAli",
	"LateBoomer",					"ruby",		"./players/LateBoomer/LateBoomer.rb",
	"Label1Goto1",					"python",	"./players/Label1Goto1/Label1Goto1.py",
	"LessSimpleShooter",			"",	"./players/LessSimpleShooter/LessSimpleShooter.pl",
	// not used - compile error
//	"TakeEmWithMe",					"vb??",		"./players/TakeEmWithMe/TakeEmWithMe.vb",
};

const int num_threads = 1;
const int num_repeats = 3;
const int num_entries = sizeof(entry_info)/sizeof(entry_info[0])/3;

int main(int argc, char **argv){
	assert(argc == 3);
	// clear results directory
	system("for fn in ./results/* ; do rm -rf \"$fn\"; done");
	
	entry *entries[num_entries];
	for(int i = 0; i < num_entries; ++i)
		entries[i] = new entry(entry_info[3*i+1], entry_info[3*i+2], entry_info[3*i]);
	
	
	auto p = new pool<standoff, num_threads, num_repeats>();
	
	// Generate every combination of 4 and push onto stack
	std::vector<bool> v(num_entries);
	std::fill(v.begin() + num_entries - 4, v.end(), true);
	int start, end;
	std::sscanf(argv[1], "%d", &start);
	std::sscanf(argv[2], "%d", &end);
	
	int which = 0;
	do{
		if(start <= which && which < end){
			entry *tmp[4]; int i = 0;
			for(int j = 0; j < num_entries; ++j)
				if(v[j])
					tmp[i++] = entries[j];
			p->push(new standoff(tmp[0], tmp[1], tmp[2], tmp[3]));
			//standoff(tmp[0],tmp[1],tmp[2],tmp[3])();
		}
		++which;
	}while(std::next_permutation(v.begin(), v.end()));
	
	p->shuffle();
	p->init();
//	process<standoff,num_threads,num_repeats>(p);
//	while(p->num_jobs()) ;// sleep(2000);
	delete p; // force join
	
	// save score
	std::ofstream scores;
	scores.open("results/scoreboard-"+std::to_string(start)+"-"+std::to_string(end)+".log");
	for(int i = 0; i < num_entries; ++i)
		scores << std::setw(30) << entries[i]->get_name() << "  " << entries[i]->get_score() << std::endl;
	scores.close();
	
	for(int i = 0; i < num_entries; ++i) delete entries[i];

	return 0;
}

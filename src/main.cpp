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
	"Bomberman",					"Rscript",	"./players/Bomberman/Bomberman.R",
	"EmoCowboy",					"python",	"./players/EmoCowboy/EmoCowboy.py",
	"Ninja",						"lua",		"./players/Ninja/ninja.lua",
	"AntiGrenadier",				"lua",		"./players/AntiGrenadier/AntiGrenadier.lua",
	"Rule0Bot",						"python2",	"./players/Rule0Bot/rule0bot.py",
	"Sniper",						"lua",		"./players/Sniper/Sniper.lua",
	// not used - compile error
//	"TakeEmWithMe",					"vb??",		"./players/TakeEmWithMe/TakeEmWithMe.vb",
	"Scared",						"python",	"./players/Scared/scared.py",
	"Neo",							"java",		"Neo",
	"CourageTheDog",				"python2",	"./players/CourageTheDog/CourageTheDog.py"
};

const int num_threads = 4;
const int num_repeats = 3;
const int num_entries = sizeof(entry_info)/sizeof(entry_info[0])/3;

int main(int argc, char **argv){
	assert(argc == 3);
	// clear results directory
	system("rm -rf ./results/*");
	
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

	// sort entries by score
	std::sort(&entries[0], &entries[num_entries], [](const entry *lhs, const entry *rhs)->bool{
		return lhs->get_score() > rhs->get_score();
	});
	
	// number of games each player played: if there are n players, each player played (n-1)choose(4-1) * num_repeats games 
	long double ngame = (num_entries-1)*(num_entries-2)*(num_entries-3)/(3*2*1) * num_repeats;
	
	// save score
	std::ofstream scores;
	scores.open("results/scoreboard.log");
	for(int i = 0; i < num_entries; ++i)
		scores << std::setw(30) << entries[i]->get_name() << "  " << entries[i]->get_score()/ngame << std::endl;
	scores.close();
	// laziness
	system("echo Generated: `date \"+%Y/%m/%d %H:%M:%S.%N UTC\"` >> results/scoreboard.log"); 
	
	for(int i = 0; i < num_entries; ++i) delete entries[i];

	return 0;
}

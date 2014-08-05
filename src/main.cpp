#include "pool.h"
#include "entry.h"
#include "standoff.h"

#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>

// minimum 4
const char *entry_info[] = { 
	"WhatsHapppening1",			"./players/WhatsHappening/WhatsHappening.sh", 
	"WhatsHapppening2",			"./players/WhatsHappening/WhatsHappening.sh", 
	"WhatsHapppening3",			"./players/WhatsHappening/WhatsHappening.sh", 
	"WhatsHapppening4",			"./players/WhatsHappening/WhatsHappening.sh", 
};

const int num_threads = 1;
const int num_repeats = 3;
const int num_entries = sizeof(entry_info)/sizeof(entry_info[0])/2;

int main(){
	// clear results directory
	system("rm -rf ./results/*");
	
	entry *entries[num_entries];
	for(int i = 0; i < num_entries; ++i)
		entries[i] = new entry(entry_info[2*i+1], entry_info[2*i]);
	
	
	auto p = new pool<standoff, num_threads, num_repeats>();
	
	// Generate every combination of 4 and push onto stack
	std::vector<bool> v(num_entries);
	std::fill(v.begin() + num_entries - 4, v.end(), true);
	do{
		entry *tmp[4]; int i = 0;
		for(int j = 0; j < num_entries; ++j)
			if(v[j])
				tmp[i++] = entries[j];
		p->push(new standoff(tmp[0], tmp[1], tmp[2], tmp[3]));
		//standoff(tmp[0],tmp[1],tmp[2],tmp[3])();
	}while(std::next_permutation(v.begin(), v.end()));
	
	// Wait for all to finish
	while(p->num_jobs());
	delete p;

	// sort entries by score
	std::sort(&entries[0], &entries[num_entries], [](const entry *lhs, const entry *rhs)->bool{
		return lhs->get_score() < rhs->get_score();
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

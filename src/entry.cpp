#include "entry.h"
#include "move.h"

#include <string>
#include <atomic>
#include <cstdio>

entry::entry(const char *command, const char *name):
	command(command), name(name), score(0) {}

move entry::run(int which, const std::string &args) const {
	std::string cmd = "timeout 1 " + std::string(command) + " " + std::to_string(which) + " \"" + args + "\" ";
	std::FILE *f = popen(cmd.c_str(), "r");
	if(!f){
		fprintf(stderr,"Could not execute `%s`\n", cmd.c_str());
		return move(move::NONE, 0);
	}else{
		int target = 0; 
		char action;
		fscanf(f, "%c%d", &action, &target);
		if(target < 0 || target > 3) target = 0;
		
		pclose(f);
		
		switch(action){
			case 'S': return move(move::SHOOT, target);
			case 'D': return move(move::DODGE, target);
			case 'P': return move(move::PREPARE, 0);
			case 'T': return move(move::THROW, target);
			default:  return move(move::NONE, 0);
		}
	}
}

void entry::add_score(int s){
	score += s;
}

const char *entry::get_name() const { return name; }
const int entry::get_score() const { return score; }

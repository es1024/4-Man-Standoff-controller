#include "entry.h"
#include "move.h"
#include "popen_noshell.h"
#include <string>
#include <atomic>
#include <cstdio>
#include <vector>

entry::entry(const char *command, const char *arg, const char *name):
	command(command), arg(arg), name(name), score(0) {}

move entry::run(int which, const std::string &args) const {
	std::vector<const char *> v;
	v.push_back("timeout");
	v.push_back("1"); 
	if(*command) v.push_back(command);
	v.push_back(arg);
	v.push_back(std::to_string(which).c_str());
	v.push_back(args.c_str());
	v.push_back(nullptr);
//std::printf("/%s %s/\n", command, arg);
	popen_noshell_pass_to_pclose p;
	std::FILE *f = popen_noshell("timeout", &v[0], "r", &p, 0);
	if(!f){
		return move(move::NONE, 0);
	}else{
		int target = 0; 
		char action;
		fscanf(f, "%c%d", &action, &target);
		if(target < 0 || target > 3) target = 0;
		
		pclose_noshell(&p);
		
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

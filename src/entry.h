#ifndef ENTRY_H
#define ENTRY_H

#include <string>
#include <atomic> 

class move;
class entry{
	const char *command, *name;
	std::atomic<int> score;
	
	public:
		entry(const char *command, const char *name);
		entry(const entry &) = default;
		
		move run(int id, const std::string &args) const;
		void add_score(int s);
		
		const char *get_name() const;
		const int get_score() const;
};

#endif

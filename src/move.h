#ifndef MOVE_H
#define MOVE_H

#include <string>

struct move{
	enum action{
		NONE = 0, SHOOT, DODGE, PREPARE, THROW,
	} act;
	int target;
	
	move() = default;
	move(action a, int t): act(a), target(t){}
	move(const move &) = default;
	
	bool operator==(const move &other) const {
		return act == other.act && target == other.target;
	}
	
	std::string to_string() const {
		switch(act){
			case SHOOT: return std::string("S") + (char)('a'+target);
			case DODGE: return std::string("D") + (char)('a'+target);
			case PREPARE: return "P";
			case THROW: return std::string("T") + (char)('a'+target);
			default: return "N";
		}
	}
};

#endif

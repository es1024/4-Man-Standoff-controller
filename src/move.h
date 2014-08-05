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
			case SHOOT: return "S" + std::to_string(target);
			case DODGE: return "D" + std::to_string(target);
			case PREPARE: return "P";
			case THROW: return "T" + std::to_string(target);
			default: return "N";
		}
	}
};

#endif

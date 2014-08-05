#ifndef STANDOFF_H
#define STANDOFF_H

class entry;
class standoff{
	entry *entries[4];
	bool initialized;
	public:
		standoff();
		standoff(entry *e1, entry *e2, entry *e3, entry *e4);
		void operator()(int which);
		bool is_initialized() const;
		
		const int SHOOT_DAMAGE =   2, // damage done by shooting
				 SELF_SHOOT_HP =   2, // damage done by shooting self is (currenthp+SELF_SHOOT_HP)
		 GRENADE_TARGET_DAMAGE =   8, // damage done to targetted player
		GRENADE_GENERAL_DAMAGE =   3, // damage done to nontargetted player
	   GRENADE_UNTHROWN_DAMAGE =   6, // damage done to player with unthrown grenade
			   STARTING_HEALTH =   5, // starting health for each player
			ZERO_HEALTH_POINTS =  35, // points if you end with 0 health
			 SOLE_WINNER_BONUS =   2, // bonus if you are the only winner (no ties)
			 TIED_WINNER_BONUS =   1, // bonus if you are a winner and there is a tie
			         MAX_TURNS =  50, // max turns before truck comes in
				  TRUCK_DAMAGE =  20; // damage done by truck
};

#endif

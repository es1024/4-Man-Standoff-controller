#include "standoff.h"
#include "entry.h"
#include "move.h"

#include <string>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <algorithm>

standoff::standoff(): entries{}, initialized(false) {}

standoff::standoff(entry *e1, entry *e2, entry *e3, entry *e4):
	entries{e1,e2,e3,e4}, initialized(true) {}

void standoff::operator()(int which){
	int health[4]={STARTING_HEALTH,STARTING_HEALTH,STARTING_HEALTH,STARTING_HEALTH};
	int turn = 0;
	std::string mvs[4]; // string representing past moves
	
	int stop[4]={}; // action is NONE unless stop[i]=0
	int prep[4]={-1,-1,-1,-1}; // cannot throw unles prep[i] > 0. self-destruct on prep[i]==0
	
	#define do4 for(int i = 0; i < 4; ++i)
	while((health[0] > 0)+(health[1] > 0) + (health[2] > 0) + (health[3] > 0) > 1 && turn < MAX_TURNS){
		// convert health to string for calling bots
		std::string hps[4];
		do4 hps[i] = std::to_string(health[i]);

		// call bots
		move mv[4];
		#define ARG(i,j,k,l) mv[i] = entries[i]->run(id*10+which, hps[i]+mvs[i]+" "+hps[j]+mvs[j]+" "+hps[k]+mvs[k]+" "+hps[l]+mvs[l]); \
		switch(mv[i].target){ \
			case 0: mv[i].target = i; break; \
			case 1: mv[i].target = j; break; \
			case 2: mv[i].target = k; break; \
			case 3: mv[i].target = l; break; \
		}
		// mixed ordering of targets
		int a = which % 3, b = (which + 1) % 3, c = (which + 2) % 3;
		ARG(0, a+1, b+1, c+1);
		ARG(1, a<1?a:a+1, b<1?b:b+1, c<1?c:c+1);
		ARG(2, a<2?a:a+1, b<2?b:b+1, c<2?c:c+1);
		ARG(3, a, b, c);
		// new hp
		int new_health[4] = {health[0], health[1], health[2], health[3]};
		// check stop[i]
		do4 if(stop[i] > 0) mv[i].act = move::NONE;
		// process action
		do4 switch(mv[i].act){
			case move::SHOOT:
				stop[i] += 2;
				new_health[mv[i].target] -= mv[i].target == i ? health[i] + SELF_SHOOT_HP : SHOOT_DAMAGE;
			break;
			case move::DODGE:
				new_health[i] += mv[mv[i].target] == move(move::SHOOT, i) ? SHOOT_DAMAGE : 0;
			break;
			case move::PREPARE:
				if(prep[i] < 0) prep[i] += 4;
				else mv[i].act = move::NONE;
			break;
			case move::THROW:
				if(prep[i] > 0){
					do4 new_health[i] -= GRENADE_GENERAL_DAMAGE;
					new_health[mv[i].target] -= GRENADE_TARGET_DAMAGE-GRENADE_GENERAL_DAMAGE;
					prep[i] = -1;
				}else mv[i].act = move::NONE;	
			default:
			break;
		}
		// stop, prepare counters
		do4 if(stop[i] > 0) --stop[i];
		do4 if(prep[i] > 0) --prep[i];
		// failure to throw while alive
		do4 if(prep[i] == 0){
			new_health[i] -= GRENADE_UNTHROWN_DAMAGE-GRENADE_GENERAL_DAMAGE;
			do4 new_health[i] -= GRENADE_GENERAL_DAMAGE;
			prep[i] = -99999;
		}
		// killed before throwing grenade
		// do4 do4: one player's posthumous explosion may kill another and cause another posthumous explosion
		do4 do4 if(prep[i] >= 0 && new_health[i] <= 0){
			new_health[i] -= GRENADE_UNTHROWN_DAMAGE-GRENADE_GENERAL_DAMAGE;
			do4 new_health[i] -= GRENADE_GENERAL_DAMAGE;
			prep[i] = -99999;
		}
		// copy over changes to health, avoid damaging dead bodies
		do4 if(health[i] > 0) health[i] = new_health[i];
		
		// update mvs string
		do4 mvs[i] += "," + mv[i].to_string();
		++turn;
	}
	// truck?
	if(turn == MAX_TURNS && ((health[0] > 0) + (health[1] > 0) + (health[2] > 0) + (health[3] > 0)) > 1)
		do4 if(health[i] > 0) health[i] -= TRUCK_DAMAGE;
	
	int max_health = std::max(std::max(health[0],health[1]),std::max(health[2],health[3]));
	// count number of winners
	int num_win = 0;
	do4 if(max_health == health[i]) ++num_win;
	// give points
	do4{ if(max_health == health[i]){
			if(num_win == 1)
				entries[i]->add_score(SOLE_WINNER_BONUS + ZERO_HEALTH_POINTS + health[i]);
			else
				entries[i]->add_score(TIED_WINNER_BONUS + ZERO_HEALTH_POINTS + health[i]);
		}else entries[i]->add_score(ZERO_HEALTH_POINTS + health[i]);
	}
	
	// log entire standoff - commented out because total size of logs scales terribly with more players
/*	std::ofstream log;
	std::string logfile = "results/" + std::string("standoff-") + entries[0]->get_name() + "-" + entries[1]->get_name() + "-" + entries[2]->get_name() + "-" + entries[3]->get_name() + "-" + std::to_string(which) + ".log";
	log.open(logfile);
	log << "Standoff between:";
	do4 log << " " << entries[i]->get_name();
	log << '\n';
	do4 log << std::setw(30) << entries[i]->get_name() << ": " << mvs[i].substr(1) << '\n';
	log << "Final Health:\n";
	do4 log << std::setw(30) << entries[i]->get_name() << ": " << health[i] << "\n";
	log << "<Player>: <Enemy 1> <Enemy 2> <Enemy 3>\n";
	#define logord(a,b,c,d) \
		log << entries[a]->get_name() << ": " << entries[b]->get_name() << " " << entries[c]->get_name() << " " << entries[d]->get_name() << "\n";
	int a = which % 3, b = (which + 1) % 3, c = (which + 2) % 3;
	logord(0,a+1,b+1,c+1);
	logord(1,a+!(a<1),b+!(b<1),c+!(c<1));
	logord(2,a+!(a<2),b+!(b<2),c+!(c<2));
	logord(3,a,b,c);
	log.close();
	system(("echo Generated: `date \"+%Y/%m/%d %H:%M:%S.%N UTC\"` >> " + logfile).c_str()); */
}

bool standoff::is_initialized() const { return initialized; }

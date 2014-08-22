#!/usr/bin/env python

import sys
import random

## ==== Player Model ================================================ ##
class Player:
    def __init__ (self, number, health):
        self.number = number
        self.health = health

def player_from (number, player_string):
    x = player_string.split (",")
    health = int (x[0].strip ())

    return Player (number, health)

def players_from (game_state):
    return [player_from (n, p) for (n, p) in
                                   zip (range(4), game_state.split ())]

def is_alive (player):
    return 0 < player.health

def i_am_dead (me):
    return not is_alive (me)

def players_who_can_breathe (players):
    return [p for p in players if is_alive (p)]

def players_by_health (players):
    return sorted (players, key=lambda p: p.health)

def least_healthy_players (players):
    sorted_living_players = \
        players_by_health (players_who_can_breathe (players))
    lowest_living_health = sorted_living_players[0].health
    return [p for p in players if lowest_living_health == p.health]
## ================================================ Player Model ==== ##

## ==== Actions ===================================================== ##
def shoot_randomly_at (possible_targets):
    chosen_target = random.choice (possible_targets)
    return "S{0}".format (chosen_target.number)

def do_nothing ():
    return "N"

def pick_move (game_state):
    players = players_from (game_state)
    me = players[0]
    enemies = players[1:]

    if i_am_dead (me):
        return do_nothing ()

    least_healthy_enemies = least_healthy_players (enemies)
    return shoot_randomly_at (least_healthy_enemies)
## ===================================================== Actions ==== ##

if "__main__" == __name__:

    game_state = sys.argv[2]
    print (pick_move (game_state))

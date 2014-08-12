#!/usr/bin/env python
import sys
import random

## ==== Move Types ================================================== ##
def move_type (move):
    if "" == move:
        return "N"
    return move[0]

def is_passive_move (move):
    if "N" == move:
        return True
    if "D" == move_type (move):
        return True
    return False

def is_aggressive_move (move):
    return not is_passive_move (move)

def passive_moves (moves):
    return [m for m in moves if is_passive_move (m)]

def aggressive_moves (moves):
    return [m for m in moves if is_aggressive_move (m)]
## ================================================== Move Types ==== ##

## ==== Player Model ================================================ ##
class Player:
    def __init__ (self, number, health, moves):
        self.number = number
        self.health = health
        self.moves  = moves

    def last_move (self):
        if 0 == len (self.moves):
            return ""
        return self.moves[-1]

def player_from (number, player_string):
    x = player_string.split (",")
    health = int (x[0].strip ())
    moves = [move.strip () for move in x[1:]]

    return Player (number, health, moves)

def players_from (game_state):
    return [player_from (n, p) for (n, p) in
                                   zip (range(4), game_state.split ())]

def is_alive (player):
    return 0 < player.health

def i_am_dead (me):
    return not is_alive (me)

def can_shoot (player):
    return "S" != move_type (player.last_move ())

def is_passive (player):
    passive_move_count = len (passive_moves (player.moves))
    aggressive_move_count = len (aggressive_moves (player.moves))

    return passive_move_count > (aggressive_move_count + 1)

def players_who_can_breathe (players):
    return [p for p in players if is_alive (p)]

def players_who_can_shoot (players):
    return [p for p in players if can_shoot (p)]

def players_who_stand_around (players):
    return [p for p in players if is_passive (p)]
## ================================================ Player Model ==== ##

## ==== Actions ===================================================== ##
def shoot_randomly_at (possible_targets):
    chosen_target = random.choice (possible_targets)
    return "S{0}".format (chosen_target.number)

def dodge_one_of_the (potential_shooters):
    chosen_shooter = random.choice (potential_shooters)
    return "D{0}".format (chosen_shooter.number)

def do_nothing ():
    return "N"

def pick_move (game_state):

    players = players_from (game_state)
    me = players[0]
    enemies = players[1:]

    if i_am_dead (me):
        return do_nothing ()

    living_enemies = players_who_can_breathe (enemies)
    if 1 == len (living_enemies):
        return shoot_randomly_at (living_enemies)

    passive_enemies = players_who_stand_around (living_enemies)
    if len (living_enemies) == len (passive_enemies):
        return shoot_randomly_at (passive_enemies)

    potential_shooters = players_who_can_shoot (living_enemies)
    if 0 < len (potential_shooters):
        return dodge_one_of_the (potential_shooters)

    return do_nothing ()
## ===================================================== Actions ==== ##

if "__main__" == __name__:

    game_state = sys.argv[2]
    print (pick_move (game_state))

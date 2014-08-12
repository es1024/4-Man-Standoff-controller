import sys
import random


def is_alive(player):
    return int(player.split(",")[0]) > 0


# Anyone with a live grenade who is alive is dangerous
def is_dangerous(player):
    return player.count("P") > player.count("T") and \
        int(player.split(",")[0]) > 0


def health(player):
    return int(player.split(",")[0])


# Failing that, healthy people are dangerous
def danger_rating(player):
    return 6 if is_dangerous(player) else health(player)

enemies = sys.argv[2].split()[1:]

highest_danger = max(danger_rating(enemy) for enemy in enemies)
most_dangerous_enemy = random.choice(
    [enemy_num+1 for enemy_num in range(len(enemies))
     if danger_rating(enemies[enemy_num]) == highest_danger])

print("S"+str(most_dangerous_enemy))

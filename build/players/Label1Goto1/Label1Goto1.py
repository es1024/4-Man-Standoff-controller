def IsAlive(player):
  return int(player[1].split(",")[0]) > 0

import sys
info_list = sys.argv[2].split()
current_turn = len(info_list[0].split(","))
action = "N"

target_list = []
for player in zip(range(1,4),info_list[1:]):
    if IsAlive(player): target_list.append(player[0])

import random
target = random.choice(target_list)

round = (current_turn - 1) % 5

if   round == 0: action = "D" + str(target)
elif round == 1: action = "P"
elif round == 2: action = "S" + str(target)
elif round == 3: action = "N"
elif round == 4: action = "T" + str(target)

print(action)

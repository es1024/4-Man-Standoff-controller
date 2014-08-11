def IsAlive(player):
  return int(player[1].split(",")[0]) > 0
def IsTarget(player, target_health):
  return int(player[1].split(",")[0]) < target_health
def HasGrenade(player):
  max_range = max(-4,-current_turn)
  for foo in range(-1,max_range,-1):
    if "P" in player[1].split(",")[foo]:
      for bar in range(-1,foo,-1):
        if player[1].split(",")[bar] not in ["T0", "T1", "T2", "T3"]:
          return True
  return False

import sys
info_list = sys.argv[2].split()
current_turn = len(info_list[0].split(","))
action = "N"

def Startgame():
  global action

  target = 1
  target_health = 5
  grenade_list=[]

  for player in zip(range(1,4),info_list[1:]):
    if HasGrenade(player):
      grenade_list.append(player)

  if not grenade_list:
    foo_list = []
    for player in zip(range(1,4),info_list[1:]):
      foo_list.append(player)
    target_list = foo_list
  else:
    target_list = grenade_list

  # Choose the least healthy player
  for player in target_list:
    if IsAlive(player) and IsTarget(player, target_health):
      target = player[0]
      target_health = int(player[1][0])

  action = "S" + str(target)

def Endgame(turn):
  global action

  if turn in [47, 49]:
    # Check if in 2 moves he can do enough damage
    rem_health = 0
    for player in zip(range(1,4),info_list[1:]):
      if IsAlive(player): rem_health += player[0]

    if rem_health < 5:
      Startgame() # It's lazy, but it should work
      return
    else:
      action = "P"
      return

  if turn in [48, 50]:
    # If Spock shot someone before, it needs to shoot again
    if info_list[0].split(",")[-1] in ["S0", "S1", "S2", "S3"]:
      Startgame()
      return
    else:
    # There's no rule against throwing grenades to dead bodies, so if
    # possible it will be thrown there.    
      target = 1
      target_health = 5

      foo_list = []
      for player in zip(range(1,4),info_list[1:]):
        foo_list.append(player)
      target_list = foo_list

      for player in target_list:
        if IsTarget(player, target_health):
          target = player[0]
          target_health = player[1][1]

      action = "T" + str(target)
      return

if current_turn > 46:
  Endgame(current_turn)
else:
  Startgame()

print(action)

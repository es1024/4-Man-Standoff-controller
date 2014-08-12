import sys

def is_alive(player):
  return int(player.split(",")[0]) > 0

closest_living_target = 1;

for player in sys.argv[2].split()[1:]:
  if is_alive(player):
    action = "S" + str(closest_living_target)
    break

  closest_living_target = closest_living_target + 1

print action

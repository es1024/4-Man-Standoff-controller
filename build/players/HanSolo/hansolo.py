import sys

def IsAlive(x):
  return int(x.split(",")[0]) > 0

closestLivingTarget = 1;

for player in sys.argv[1].split()[1:]:
  if IsAlive(player):
    action = "S" + str(closestLivingTarget)
    break

  closestLivingTarget = closestLivingTarget + 1

print action

import sys

def total_humans_alive(humans):
  return sum([is_alive(human) for human in humans])

def is_alive(x):
  return int(x.split(",")[0]) > 0  

def is_threat_to_humanity(lastAction):
  return lastAction == "P"

action = "N"
threat_id = 1
humans = sys.argv[2].split()[1:];

if total_humans_alive(humans) == 3:
  for human in humans:
    if is_threat_to_humanity(human[-1]):
      action = "S" + str(threat_id)
      break
    threat_id= threat_id+ 1

print action

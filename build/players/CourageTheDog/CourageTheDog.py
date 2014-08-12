import sys
from collections import defaultdict as ddict
from random import choice
args = sys.argv
info = " ".join(args[2:]).strip('"').split(" ")
players = ddict(dict)
for i,s in enumerate(info):
    parts = s.split(",")
    players[i]["health"]=int(parts[0])
    players[i]["last"]=parts[-1]
    players[i]["history"]=parts[1:]
    players[i]["turn"]=len(parts)
me=0
others=[1,2,3]
turn=players[me]["turn"]
alive = filter(lambda p:players[p]["health"]>0,others)
def act():
    if turn is 1:
        return "S%d" % choice(alive)
    if "P" in players[me]["history"][-3:]:
        return "T%d" % choice(alive)
    for i in others:
        if players[i]["last"] is "P":
            return "P"
    if choice([True,False,False]):
        return "S%d" % choice(alive)
    return "D%d" % choice(alive)
print act()

import sys    

def ident(thatguy):

    return int(thatguy.split(",")[0])

def health(thatguy):
    return int(thatguy.split(",")[1])

def shooter(thatguy):
    if(len(thatguy.split(","))<3):
        return 1==1
    else: return thatguy.split(",")[2][0]=="S"

def mosthealth(group):
    bigbad=group[0]
    for foe in group:
        if (health(foe)>health(bigbad)): bigbad=foe
    return bigbad

def igotanuke(mine):
    return mine.count("P")-mine.count("T")>0

def guytonuke(allguys,fighters):
    backup=allguys[:]
    for Aguy in backup:
        if(health(Aguy)<4):
            allguys.remove(Aguy)
            if (shooter(Aguy)): fighters.remove(Aguy)

    if(len(allguys)==0): return mosthealth(backup)
    if (len(allguys)==len(fighters)):
        return mosthealth(allguys)
    else:
        for fighter in fighters: allguys.remove(fighter)
        return mosthealth(allguys)

raw = sys.argv[2]
player = raw.split(" ")
thisisme=player.pop(0)
turn = len(player[0].split(","))-1

guys=[]
gunners=[]
c=1
for dude in player:
    dude=str(c)+","+dude
    c+=1
    if (health(dude)>0): 
        guys.append(dude)
        if (shooter(dude)):
            gunners.append(dude)

if (turn==0): print "P"
elif(turn==49): print"S0"
elif(igotanuke(thisisme))&( turn % 2 == 1): print "T"+str(ident(guytonuke(guys,gunners)))
elif(len(guys)<2)&(len(gunners)>0) & (turn % 2 == 1): print "P"
elif(turn % 2 == 0) & (len(gunners)>0): print "D"+str(ident(mosthealth(gunners)))
elif(turn % 2 == 1) & (len(gunners)>0): print "S"+str(ident(mosthealth(gunners)))
else: print "S"+str(ident(mosthealth(guys)))

import sys,re,random

history = sys.argv[2]
history = [k.split(",") for k in history.split()]
health = [k[0] for k in history]
last_turn = [k[-1] for k in history]
last_shooter = [i for i,x in enumerate(last_turn) if re.search(r'S[0-3]',x)]
last_prepare = [i for i,x in enumerate(history[0]) if x=='P']
if not len(last_prepare):
    last_prepare = [-1]

last_throw = [i for i,x in enumerate(history[0]) if re.search(r'T[0-3]',x)]
if not len(last_throw):
    last_throw = [0]

most_healthy = [i for i,x in enumerate(health) if x==max(health)]
dead = [i for i,x in enumerate(health) if x<=0]
inoffensive = last_shooter+dead
danger = [k for k in range(1,4) if k not in inoffensive]
alive = [k for k in range(1,4) if k not in dead]
if health[0]>3 and last_throw[-1] > last_prepare[-1]:
    out = 'P'

if last_throw[-1] < last_prepare[-1]:
    if len(last_shooter):
        out = 'T'+random.choice(last_shooter)
    else:
        out = 'T'+random.choice(most_healthy)

if health[0]<=3 and last_throw[-1] > last_prepare[-1]:
    if len(danger):
        out = 'D'+random.choice(danger)
    else:
        out = 'S'+random.choice(alive)

print(out)

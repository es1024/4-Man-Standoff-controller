import sys

def health(p):
    return int(p[0])

def is_alive(p):
    return health(p) > 0

def can_act(p):
    return is_alive(p) and p[-1][0] != 'S'

def can_throw(p):
    return is_alive(p) and p[-1][0] == 'P'

def shot_first(p):
    if len(p) == 1:
        return False
    return p[1][0] == 'S'

def act(a):
    print a
    sys.exit(0)

player = sys.argv[2].split()[0].split(',')
enemies = [e.split(',') for e in sys.argv[2].split()[1:]]
healthiest = sorted(enumerate(enemies, 1), key=lambda e:health(e[1]))[-1]
alive = sum(is_alive(e) for e in enemies)

if alive == 1:
    i, e = healthiest
    if health(e) <= 2 and not can_act(e):
        act('S%d' % i)
    if can_throw(player):
        act('T%d' % i)
    if can_throw(e):
        act('S%d' % i)
    if can_act(e) and shot_first(e) and len(player) < 40:
        act('D%d' % i)
    if len(player) > 45:
        act('P')
    act('S%d' % i)

if can_throw(player):
    i, e = healthiest
    act('T%d' % i)

if len(player) > 45:
    act('P')

if health(player) <= 2 or any(can_throw(e) for e in enemies) or alive == 2:
    i, e = healthiest
    act('S%d' % i)

act('P')

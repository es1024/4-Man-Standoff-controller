def health(guy)
    return guy.split(",")[1].to_i
end

def bomb(past)
    return past.count("P")-past.count("T")>0    
end

def maxhealth(players)
    target=players[0]
    for dude in players
        if health(dude)>health(target)
            target=dude
        end
    end
    return target
end

def reloading(past)
    return past[past.length-2]=="S"
end

input=ARGV[1]
them = input.split(" ")
me = "0,"+them.delete_at(0)
turn = me.split(",").length-1

x=0
y=0

while y<them.length do
    x +=1
    them[y]=x.to_s+","+them[y]
    if health(them[y])<1
        them.delete_at(y)
        y -=1
    end
    y +=1
end

if turn==1 
    print "D1"
elsif bomb(me)
    print "T"+ maxhealth(them)[0][0].to_s
elsif turn>2 and not(bomb(input)) and them.length>1 and health(me)>2 and not(reloading(me))
    print "P"
elsif bomb(input)
    print "S"+maxhealth(them)[0][0].to_s
else
    print "S"+them[0][0]
end 

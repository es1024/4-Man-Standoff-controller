--Don't Nuke Me Bro

math.randomseed(os.time())

info=arg[1]

function pTable(t, c) -- Debugging
    p = c or ""
    for k,v in pairs(t) do
        if type(v)=="number"  then
            p = p .. " " .. v
        elseif type(v)=="table" then
            p = p .. tostring("\n"..pTable(v))
        elseif type(v)=="string" then
            p = p .. "\"" .. v .. "\""
        end
    end
    p=p.."}"
    return "{"..    p
end

function parseEnemies(s) -- Makes sense of the input Data
    local e={}
    local pStatus={{},{},{},{}}
    for w in string.gmatch(s, "%S+") do table.insert(e,w) end

    for p = 1, 4 do
        for a in string.gmatch(e[p], "%P+") do
            table.insert(pStatus[p], a)
        end
    end
    for i = 1 , 4 do
        pStatus[i][1]=tonumber(pStatus[i][1])
    end
    return pStatus
end

function analyseEnemies(ps)
    moveList={{},{}}
    sFlag = 0
    for i=2,4 do
        pHealth = ps[i][1]
        lan=table.getn(ps[i])
        lAction=ps[i][lan]

        if pHealth <= 0 then
        else
            if lAction=="P" then -- Doesn't Like it when people goes nuclear
                table.insert(moveList[1], tostring("S"..i-1))
                h=8-pHealth
                table.insert(moveList[2], h)
            end

            if lAction=="S0" then -- takes notice when someone shoots at him
                sFlag = sFlag +1
                if pHealt == 2 or 1 then
                    table.insert(moveList[1],tostring("S".. i-1))
                    table.insert(moveList[2], 2)
                else
                    table.insert(moveList[1],tostring("D"..i-1))
                    table.insert(moveList[2], 3)
                end
            end

        end
    end
    nmla = table.getn(ps[1])

    if nmla == 2 and ps[1][nmla] == "P" then -- gotta get rid of this grenade
        tH= 0
        target = 1
        for i = 2, 4 do
            if ps[i][1]>=tH then
                target= i-1
            end
        end
        table.insert(moveList[1],tostring("T"..target))
        table.insert(moveList[2], 6)
    end


    if nmla == 3 and ps[1][nmla-1] == "P" then -- Makes sure he throws the damn grenade
        tH= 0
        target = 1
        for i = 2, 4 do
            if ps[i][1]>=tH then
                target= i-1
            end
        end
        table.insert(moveList[1],tostring("T"..target))
        table.insert(moveList[2], 20)

    end

    if sFlag >=2 then -- if 2 or more people shoot at him during a round, he goes nuclear.
        table.insert(moveList[1],tostring("P"))
        table.insert(moveList[2], 9)
    end
    r=math.random(3)
    table.insert(moveList[1], "D"..r) -- random ducking if all else fails
    table.insert(moveList[2], 1)
    return moveList
end

function makeMove(mList) -- picks the highest rated move
    m = ""
    nml = table.getn(mList[2])
    c = 0
    m = 1
    for i = 1, nml do
        if mList[2][i]>= c then
            c = mList[2][i]
            m = mList[1][i]
        end
    end
    print(m)
end

makeMove(analyseEnemies(parseEnemies(info)))

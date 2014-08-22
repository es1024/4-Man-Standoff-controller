turns = arg[2]
health = string.sub(turns, 1, 1)
--make random numbers random
math.randomseed(io.popen("date +%s%N"):read("*all"))
math.random(); math.random(); math.random()
function Split(str, delim, maxNb)
    -- Eliminate bad cases...
    if string.find(str, delim) == nil then
        return { str }
    end
    if maxNb == nil or maxNb < 1 then
        maxNb = 0    -- No limit
    end
    local result = {}
    local pat = "(.-)" .. delim .. "()"
    local nb = 0
    local lastPos
    for part, pos in string.gmatch(str, pat) do
        nb = nb + 1
        result[nb] = part
        lastPos = pos
        if nb == maxNb then break end
    end
    -- Handle the last field
    if nb ~= maxNb then
        result[nb + 1] = string.sub(str, lastPos)
    end
    return result
end
enemies = Split(turns, " ")
--first turn
if #enemies[1] == 1 then
  print(string.format("S%i",math.random(1,3)))
  os.exit()
end
--kills if possible
for enemy=1,3 do
  if (tonumber(string.sub(enemies[enemy + 1],1,1)) or 0) < 3 and string.sub(enemies[enemy + 1],1,1) ~= "-" then
    print(string.format("S%i",enemy))
    os.exit()
  end
end
--shoots the last person that shot at it
for enemy=1,3 do
  if string.sub(enemies[enemy + 1],#enemies[enemy + 1]-1) == "S0" and tonumber(string.sub(enemies[enemy + 1],1,1)) > 0 then
    print(string.format("S%i",enemy))
    os.exit()
  end
end
--otherwise shoot a random alive person
local aliveEnemies = {}
for enemy=1,3 do
  if string.sub(enemies[enemy + 1],1,1) ~= "-" then
    aliveEnemies[#aliveEnemies+1]=enemy
  end
end
print(string.format("S%i",math.random(1,#aliveEnemies)))

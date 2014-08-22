function string:split( inSplitPattern, outResults )
   if not outResults then
      outResults = { }
   end
   local theStart = 1
   local theSplitStart, theSplitEnd = string.find( self, inSplitPattern, theStart )
   while theSplitStart do
      table.insert( outResults, string.sub( self, theStart, theSplitStart-1 ) )
      theStart = theSplitEnd + 1
      theSplitStart, theSplitEnd = string.find( self, inSplitPattern, theStart )
   end
   table.insert(outResults, string.sub( self, theStart ) )
   return outResults
end

function RunRound()
   -- set up the players
   players = match:split(" ")
   myHealth = string.sub(players[1],1,1)
   closestHealth = 10
   enemy = 3
   if myHealth == "-" then
      return "N"
   end

   -- find someone with health close to mine
   for i=2,#players do
      moves = players[i]
      playerHealth = moves:sub(1,1)
      if playerHealth ~= "-" then
         if math.abs(playerHealth - myHealth) <= closestHealth then
            enemy = i-1
            closestHealth = math.abs(playerHealth - myHealth)
         end
      end
   end
   return "S"..enemy
end

local args = {...}  -- command line arguments
match = args[2]     -- store the set of matches
action = RunRound()
print(action)

-- Antigrenadier
local args = {...}  -- command line arguments

match = args[2]     -- store the set of matches

-- why this isn't standard in Lua....
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
  table.insert( outResults, string.sub( self, theStart ) )
  return outResults
end

-- set up the players
players = match:split(" ")

-- search other players for someone who pulled a grenade
for i=2,#players do
   moves = players[i]
   -- test if person is alive
   if moves:sub(1,1) ~= "-" then
      -- cycle through all elements of the string
      for j=#moves,2,-1 do
         -- if found, shoot!
         if moves:sub(j,j) == "P" then
            print("S"..i-1)
            os.exit()
         end
      end
   end
end

-- otherwise we just relax
print("N")

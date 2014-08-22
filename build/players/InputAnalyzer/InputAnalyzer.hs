import System.Environment   
import Data.Char (ord)
import Data.List.Split

main = do 
    args <- getArgs
    let secondArg = (last args)
    let opt = (argCount secondArg 0)
    let list = (splitOn " " secondArg)
    let enemysToCheck = [1,2,3]
    let target = (avoidShootingSelf (findTarget (last args) 0 0 0 0))
    putStrLn (decide list enemysToCheck opt target)

argCount :: String -> Int -> Int
argCount (s:str) i
    |(length str) == 0 = i `mod` 4
    | otherwise = (argCount str (i + (ord s)))

--myPseudo takes number 0-3, and a possible target and translates it to a command 
myPseudo :: Int -> Int -> String
myPseudo 0 b = "S" ++ (show b)
myPseudo 1 b = "D" ++ (show b)
myPseudo 2 b = "P"
myPseudo 3 b = "T" ++ (show b)

decide :: [String] -> [Int] -> Int -> Int -> String
decide [] [] a b = (myPseudo a b)
decide (x:xs) [] a b = (myPseudo a b)
decide xs (y:ys) a b
    | (liveGrenade z 0) == True = "D" ++ (show y)
    | otherwise = (decide xs ys a b)
    where z = xs!!y

--checks if a player has a live grenade
liveGrenade :: String -> Int -> Bool
liveGrenade [] a = a > 0
liveGrenade (s:str) a
    | s == 'T' = (liveGrenade str (a - 1))
    | s == 'P' = (liveGrenade str (a + 1))
    | otherwise = (liveGrenade str a)

--findTarget picks a target by doing some irrelevant string processing on the 2nd argument
findTarget :: String -> Int -> Int -> Int -> Int -> Int
findTarget [] a b c d = ((maximum [a,b,c,d]) `mod` 4)
findTarget (s:str) a b c d
    | s == 'S' = (findTarget str (a + 1) b c d)
    | s == 'D' = (findTarget str a (b + 1) c d)
    | s == 'P' = (findTarget str a b (c + 1) d)
    | s == 'T' = (findTarget str a b c (d + 1))
    | s == 'N' = (findTarget str a b c (d + 1))
    | otherwise = (findTarget str a b c d)

--Makes sure I do target myself takes int 0-3
avoidShootingSelf :: Int -> Int
avoidShootingSelf 0 = 1
avoidShootingSelf a = a

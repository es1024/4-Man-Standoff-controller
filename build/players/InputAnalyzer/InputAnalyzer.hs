import System.Environment   
import Data.Char (ord)

main = do 
    args <- getArgs
    let opt = (argCount (last args) 0)
    let target = (findTarget (last args) 0 0 0 0)
    putStrLn (decide opt target)

argCount :: String -> Int -> Int
argCount (s:str) i
    |(length str) == 0 = i `mod` 4
    | otherwise = (argCount str (i + (ord s)))

--decide takes number 0-3, and a possible target and translates it to a command 
decide :: Int -> Int -> String
decide 0 b = "S" ++ (show b)
decide 1 b = "D" ++ (show b)
decide 2 b = "P"
decide 3 b = "D" ++ (show b)


--findTarget picks a target by doing some irrelevant string processing on the 2nd argument
findTarget :: String -> Int -> Int -> Int -> Int -> Int
findTarget (s:str) a b c d
    | (length str) == 0 = ((maximum [a,b,c,d]) `mod` 4) + 1
    | s == 'S' = (findTarget str (a + 1) b c d)
    | s == 'D' = (findTarget str a (b + 1) c d)
    | s == 'P' = (findTarget str a b (c + 1) d)
    | s == 'T' = (findTarget str a b c (d + 1))
    | s == 'N' = (findTarget str a b c (d + 1))
    | otherwise = (findTarget str a b c d)

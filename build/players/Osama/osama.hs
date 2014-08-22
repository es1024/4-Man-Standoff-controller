module Main where

import Data.List
import Data.Ord
import System.Environment

words' "" = []
words' s = s' : words' (tail' s'')
  where
    (s', s'') = break (==',') s
    tail' (',':r) = r
    tail' r = r

nRound = length . words'

lastAction = last . words'

health :: String -> Int
health = read . head . words'

alive = (>0) . health

grenadeAge :: String -> Int
grenadeAge p | not (alive p) = 0
             | otherwise = g 0 $ tail $ words' p
  where
    g n (a:b:r) | head a == 'S' = g (if n>0 then n+2 else 0) r
    g 0 ("P":r) = g 1 r
    g n (('T':_):r) | n>0 = g 0 r
    g n (_:r) | n>0 = g (n+1) r
    g n (_:r) = g n r
    g n [] = n

prepared :: String -> Bool
prepared p = alive p && head (lastAction p) /= 'S'

nShotMe = length . filter (=="S0") . words'

getPlayer = (!!)

action players@(me:them) | not (prepared me) = "S2" -- bogus
                         | nRound me >= 49 = "S0"
                         | grenadeAge me >= 1 = 'T':(show $ maximumBy (comparing (nShotMe . getPlayer players)) l)
                         | any prepared them && nRound me > 0 = 'D':(show $ maximumBy (comparing (nShotMe . getPlayer players)) l)
                         | otherwise = 'S':(show $ maximumBy (comparing (health . getPlayer players)) l)
  where l = filter (alive . (getPlayer players)) [1..3]



main = do
  players <- fmap (words . head . tail) getArgs
  putStrLn $ action players

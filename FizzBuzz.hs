module Main(main) where

import List

f :: Int -> String
f x | (x `mod` 15 == 0) = " fizzbuzz"
    | (x `mod` 5 == 0) = " buzz"
    | (x `mod` 3 == 0) = " fizz"
f 1 = "1"
f x = ' ':(show x)

main :: IO ()
main = (putStr.concat) (map f [1..100])

-- Another version (with similar performance characteristics)
-- main' :: IO ()
-- main' = mapM_ putStr (map f [1..100])

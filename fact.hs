module Main where

-- fact :: Integer -> Integer
fact 0 = 1
fact n = n * fact (n - 1)

f_print n = print(show n ++ "th factorial is " ++ show (fact n))
main = f_print 15

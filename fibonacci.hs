module Main where
f n | n <  2 = 1
    | n >= 2 = f (n-1) + f (n-2)
f_print n = print(show n ++ "th Fibonacci number is " ++ show (f n))
main = f_print 15

{-
Simple Haskell program to solve the Four Fours puzzle.
Namely: express each integer between 1 and 100   
using standard operations and exactly four fours
http://www.joachim-breitner.de/various/fourfours.hs

Copyright: Â© 2006 Joachim Breitner <mail@joachim-breitner.de

Inspired by the python version by Rich Burridge:
http://blogs.sun.com/richb/resource/fourfours.py
Copyright Â© 2006 Rich Burridge
which is based on the C# version by Carl Johansen
http://www.carljohansen.co.uk/fourfours/fourfours.cs.txt
Copyright Â© 2005 Carl Johansen
-}

import List
import Maybe

-- Data types to build up our Terms
data Value = Four | SqrtFour | FourBang | PointFour | PointFourBar | SqrtPointFourBar 
data Op    = (:+:) | (:*:) | (:-:) | (:/:) | (:^:)
data Term = Simple Value | Binary Op Term Term

-- special power for our rationals
a *** 2 = a * a
a *** 4 = a * a * a * a
a *** b = 1000000 -- hopefully invalid :-)

-- Save div (div through zero should just result in something useless)
a // 0 = 100000
a // b = a / b

-- Easy stringifying of our terms
instance Show Value where
	show Four		= "4"
	show SqrtFour		= "sqr(4)"
	show FourBang		= "4!"
	show PointFour		= ".4"
	show PointFourBar	= ".4bar"
	show SqrtPointFourBar	= "sqr(.4bar)"
instance Show Op where
	show (:+:)		= " + "
	show (:*:)		= " * "
	show (:-:)		= " - "
	show (:/:)		= " / "
	show (:^:)		= " ^ "
instance Show Term where
	show (Simple v)		= show v
	show (Binary o a b)	= "(" ++ show a ++ show o  ++ show b ++ ")"

-- Value to rational
valueOf Four			= 4
valueOf SqrtFour		= 2
valueOf FourBang		= 24
valueOf PointFour		= 4/10
valueOf PointFourBar		= 4/9
valueOf SqrtPointFourBar	= 2/3
	
-- Evaluation
eval (Simple v)		= valueOf v
eval (Binary o a b)	= runop o (eval a) (eval b)
	where	runop (:+:) = (+)
		runop (:*:) = (*)
		runop (:-:) = (-)
		runop (:/:) = (//)
		runop (:^:) = (***)

-- All possible groupings. I have five, why does the python code have only four?
groupings v1' v2' v3' v4' o1' o2' o3' = [
	 (v1 `o1`   v2) `o2` (v3   `o3` v4),
	((v1 `o1`   v2) `o2`  v3)  `o3` v4,
	 (v1 `o1`  (v2  `o2`  v3)) `o3` v4,
	  v1 `o1` ((v2  `o2`  v3)  `o3` v4),
	  v1 `o1`  (v2  `o2` (v3   `o3` v4))
	]
  where	v1 = Simple v1'
  	v2 = Simple v2'
  	v3 = Simple v3'
  	v4 = Simple v4'
	o1 = Binary o1'
	o2 = Binary o2'
	o3 = Binary o3'

-- List of all possible values and binary operations
zeroaries = [Four, SqrtFour, FourBang, PointFour, PointFourBar, SqrtPointFourBar ]
binaries  = [(:+:),(:*:),(:-:),(:/:),(:^:)]

-- Hehe. List monad. Returns all possibilities, basically
all_combos = do
	v1 <- zeroaries
	v2 <- zeroaries
	v3 <- zeroaries
	v4 <- zeroaries
	b1 <- binaries
	b2 <- binaries
	b3 <- binaries
	group <- groupings v1 v2 v3 v4 b1 b2 b3 
	return group

-- We only evaluate them once, for speed
evaled_combos = map (\t -> (eval t,t)) all_combos

-- To search for a specify number, and print the result
search_for n = case lookup (fromInteger n) evaled_combos of	
	Just found -> show n ++ ": " ++ show found
	Nothing	   -> show n ++ ": not found"

-- Search for all requested numbers and append newlines
result = unlines $ map search_for [1..100]

-- For fun: First not found number:
notfound = show $ head $ dropWhile (\n -> isJust $ lookup (fromInteger n) evaled_combos) [1..]

-- Main function
main = do putStr result >> putStrLn "And so on until: " >> putStrLn notfound


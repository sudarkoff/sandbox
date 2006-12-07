module MinCombination where
        
-- http://en.wikipedia.org/wiki/Permutations_and_combinations
combinations 0 _ = [[]]
combinations _ [] = []
combinations k (x:xs) =
    map (x:) (combinations (k-1) xs)
    ++
    combinations k xs

monthsTakings = 
    [-50,-21,  13, 171, 14,
     -42,-58, 109,   4,  7,
     -23,-44, -98,-121,101, 
      33, 87,-121, -40,-65,
      43, 54, -45, -12,-12,
      38, 25,   3,   7,  8]

main = foldr (+) 1 monthsTakings

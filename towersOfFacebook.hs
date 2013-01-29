-- hanoi for n disks and r pegs [p1, p2, ..., pr]
hanoiR :: Integer -> [a] -> [(a, a)]

-- zero disks: no moves needed.
hanoiR 0 _ = []

-- one disk: one move and two pegs needed.
hanoiR 1 (p1 : p2 : rest) = [(p1, p2)]

-- n disks and r > 3 pegs: use Frame-Stewart algorithm
hanoiR n (p1 : p2 : p3 : rest) =
   hanoiR k (p1 : p3 : p2 : rest) ++
   hanoiR (n - k) (p1 : p2 : rest) ++
   hanoiR k (p3 : p2 : p1 : rest)
   where k
            | (null rest)   = n - 1
            | otherwise     = n `div` 2

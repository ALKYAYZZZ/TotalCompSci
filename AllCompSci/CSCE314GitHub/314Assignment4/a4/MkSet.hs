type Set a = [a]
--Go through set and check to see if any element in set x is in set y
isElement:: (Eq a)=> a -> [a] -> Bool
isElement x [] = False
isElement x (y:ys) = if x == y then True else isElement x ys

mkSet :: Eq a => [a] -> Set a
--Only make a set with common elements
mkSet [] = []
mkSet (x:xs) | isElement x xs = mkSet xs
			 | otherwise = x:mkSet xs
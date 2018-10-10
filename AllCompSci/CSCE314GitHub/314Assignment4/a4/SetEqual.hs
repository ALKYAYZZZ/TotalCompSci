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

--Check each element of x to see if it is in set y and remove header to x until no elements remain			 
subset :: Eq a => Set a -> Set a -> Bool
subset [] [] = True
subset [] ys = True
subset (x:xs) ys 	| isElement x ys == True = subset xs ys
					| otherwise = False

--Check to see if both sets are subsets of one another				
setEqual :: Eq a => Set a -> Set a -> Bool
setEqual [] [] = True
setEqual [] ys = False
setEqual xs [] = False
setEqual xs ys 	   | (subset xs ys == True) && (subset ys xs == True) = True
				   | otherwise = False
				  
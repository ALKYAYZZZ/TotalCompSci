type Set a = [a]
--Go through set and check to see if any element in set x is in set y
isElement:: (Eq a)=> a -> [a] -> Bool
isElement x [] = False
isElement x (y:ys) = if x == y then True else isElement x ys

--Check each element of x to see if it is in set y and remove header to x until no elements remain			 
subset :: Eq a => Set a -> Set a -> Bool
subset [] [] = True
subset [] ys = True
subset (x:xs) ys 	| isElement x ys == True = subset xs ys
					| otherwise = False
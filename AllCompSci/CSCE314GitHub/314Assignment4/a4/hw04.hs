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
				 
--Make list of all possible element combinations, if second set only has one element then recursion is slightly modified			 
setProd :: (Eq t, Eq t1) => Set t -> Set t1 -> Set (t, t1)
setProd [] [] = []
setProd [] ys = []
setProd xs [] = []
setProd xs ys = if length ys > 1 then (head xs,head ys):(head xs,(head (tail ys))):setProd (tail xs) ys else (head xs,head ys):setProd (tail xs) ys

--Split partitions into two different functions to take partition of single elements and then partition of groups of elements
partitionSet :: Eq t => Set t -> Set( Set (Set t))
partitionSet [] = [[]]
partitionSet (x:xs) = map ([x]:) (partitionSet xs) ++ foldr (++) [][miniPartition (x:) z | z <- partitionSet xs]

miniPartition:: (t -> t) -> [t] -> [[t]]
miniPartition a [] = []
miniPartition a (x:xs) = [a x:xs] ++ map (x:) (miniPartition a xs)

--Calc length of partitionSet from 1 to the input number
bellNum :: Int -> Int
bellNum x = length(partitionSet[1..x])
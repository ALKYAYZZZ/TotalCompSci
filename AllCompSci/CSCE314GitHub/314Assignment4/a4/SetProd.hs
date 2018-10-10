--Make list of all possible element combinations, if second set only has one element then recursion is slightly modified			 
setProd :: (Eq t, Eq t1) => Set t -> Set t1 -> Set (t, t1)
setProd [] [] = []
setProd [] ys = []
setProd xs [] = []
setProd xs ys = if length ys > 1 then (head xs,head ys):(head xs,(head (tail ys))):setProd (tail xs) ys else (head xs,head ys):setProd (tail xs) ys
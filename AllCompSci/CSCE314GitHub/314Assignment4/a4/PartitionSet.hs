--Split partitions into two different functions to take partition of single elements and then partition of groups of elements
partitionSet :: Eq t => Set t -> Set( Set (Set t))
partitionSet [] = [[]]
partitionSet (x:xs) = map ([x]:) (partitionSet xs) ++ foldr (++) [][miniPartition (x:) z | z <- partitionSet xs]

miniPartition:: (t -> t) -> [t] -> [[t]]
miniPartition a [] = []
miniPartition a (x:xs) = [a x:xs] ++ map (x:) (miniPartition a xs)
module TestHelper where

import Main

helpersubset :: Eq a => Set a -> Set a -> Bool
helpersubset [] _ = True
helpersubset (x:xs) ys = x `elem` ys && helpersubset xs ys


helpersetEqual :: Eq a => Set a -> Set a -> Bool
helpersetEqual xs ys = helpersubset xs ys && helpersubset ys xs


elementOfCMY2 :: Eq a => Set a -> Set(Set a) -> Bool
elementOfCMY2 e [] = False
elementOfCMY2 e (h:ls)  | (setEqual h e)  = True
                        |  otherwise = elementOfCMY2 e ls

subsetCMY2 :: Eq a => Set(Set a) -> Set(Set a) -> Bool
subsetCMY2 [] lS = True
subsetCMY2 (f:aS) lS = elementOfCMY2 f lS && subsetCMY2 aS lS

setEqualCMY2:: Eq a => Set(Set a) -> Set(Set a) -> Bool
setEqualCMY2 aS bS = subsetCMY2 aS bS && subsetCMY2 bS aS

elementOfCMY :: Eq a => Set(Set a) -> Set(Set(Set a)) -> Bool
elementOfCMY e [] = False
elementOfCMY e (h:ls)  |  (setEqualCMY2 h e)  = True
                       |  otherwise = elementOfCMY e ls

subsetCMY :: Eq a => Set(Set(Set a)) -> Set(Set(Set a)) -> Bool
subsetCMY [] lS = True
subsetCMY (f:aS) lS = elementOfCMY f lS && subsetCMY aS lS

setEqualCMY :: Eq a => Set(Set(Set a)) -> Set(Set(Set a)) -> Bool
setEqualCMY aS bS = subsetCMY aS bS && subsetCMY bS aS


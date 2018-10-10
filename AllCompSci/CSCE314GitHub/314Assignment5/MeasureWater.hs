module Main where

measureWater :: Int -> Int -> Int -> Bool
measureWater a b c = helper a b c a b


--The function works by a series of if statements. If one of the jugs can hold zero water and
--the other jug is not the size of the wanted number of gallons, the function returns false.
--Futhermore, the function decides which jug currently has more water in it, if neither of the
--jugs are at full capacity, then the lesser full jug is emptied and is filled with all of the water in the
--jug that is more full. The jug that was just emptied is then filled to capacity with water. If the difference
--of the water in the more full jug to the less full jug is equal to the capacity of the smaller jug, the more full
--jug is emptied into the lesser full jug until it is at capacity. Finally, if none of the above conditions are satisfied
--then the less full jug is emptied and the more full jug is poured into the now emptied jug, leaving a remaining measure of
--(full jug) - (less full jug) in the originally full jug. If the two jugs have an equal amount of water in them then it is
--Impossible to make a jug of size c and the test fails.
--
helper:: Int -> Int -> Int -> Int -> Int -> Bool
helper a b c d e| ((a == 0) || (b == 0)) && ((a == c) || (b == c)) = True
				| ((a == 0) || (b == 0)) && ((a /= c) || (b /= c)) = False
				| a == c = True
				| b == c = True
				| a == b = False
				| (a /= d) && (b /= e) && (a > b) = helper b e c d e
				| (a /= d) && (b /= e) && (a < b) = helper d a c d e
				| (a > b) && (a - b == e) && (b /= e) = helper ((e - b) + a) e c d e
				| (a < b) && (b - a == d) && (a /= d) = helper d ((d - a) + b) c d e
				| a > b = helper (a - b) b c d e
				| a < b = helper a (b - a) c d e
				| otherwise = False
				


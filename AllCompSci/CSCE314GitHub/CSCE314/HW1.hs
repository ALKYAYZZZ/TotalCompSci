import Data.Char			

double :: Num a => a -> a
double x = x + x

increaseTen :: Num a => a -> a
increaseTen x = x + 10

circleArea :: (Ord a, Floating a) => a -> a
circleArea x = 
		   if x < 0
		   then 0
		   else pi * (x ^ 2)
		   
midList :: [a] -> [a]
midList [] = []
midList [x] = []
midList sxs = tail(init sxs)

countdownList x y = 
					if x >= y 
					then error "Second number higher than the first"
					else reverse[x..y]
					
isRight x y z = 
				if(x == 0 || y == 0 || z == 0)
				then error "Side can't be zero"
				else if (x^2 + y^2 == z^2)
				then True
				else False


multComplex (x,y)(z,r) = ((x * z - y * r),(x * r + y * z))

countChar y [] = 0
countChar y (c:ys)
					|y == c = 1 + countChar y ys
					|otherwise = countChar y ys
				
getFirsts ys = map fst ys

halfList [] = []
halfList [y] = [y]
halfList (y:ys) = y : halfList(tail ys)

uppercaseListHelp y     | isUpper y = (True, False, False)
						| isLower y = (False, True, False)
						| isDigit y = (False, False, True)
						| otherwise = (False, False, False)
uppercaseList ys = map uppercaseListHelp ys


altSeries :: Num a => [a] -> a
altSeries [] = 0
altSeries (x:xs) = (-1 * altSeries xs) + x

				
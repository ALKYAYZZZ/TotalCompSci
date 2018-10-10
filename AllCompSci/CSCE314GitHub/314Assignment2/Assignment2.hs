import Data.Char

myReverse:: [x] -> [x]
myReverse [] = []
myReverse (x:xs) = (myReverse xs) ++ [x]

isElement:: (Eq x)=> x -> [x] -> Bool
isElement _[] = False
isElement p (x:xs)   | p == x 	 = True
					 | otherwise = isElement p xs 

duplicate:: [x] -> [x]
duplicate [] = []
duplicate (x:[]) = [x, x]
duplicate (x:xs) = [x,x] ++(duplicate xs) 

removeDuplicate :: (Eq x)=>[x] -> [x]
removeDuplicate [] = []
removeDuplicate (x:xs) = x : removeDuplicate (filter(/= x) xs)

rotate:: [x] -> Int -> [x]
rotate xs p | p > length xs = drop (p - length xs) xs ++ take (p - length xs) xs
			| otherwise = drop p xs ++ take p xs
			
flatten:: [[x]] -> [x]
flatten [] = []
flatten (x:xs) = x ++ flatten xs

isPalindrome:: (Eq x) => [x] -> Bool
isPalindrome [] = True
isPalindrome xs 	| xs == reverse xs = True
					| otherwise = False
					
solver:: (Integral x)=>x -> x -> x
solver x y | x == 0 = y
			| y == 0 = x
			| otherwise = solver y (mod x y)
			
coprime:: (Integral x)=>(Eq x)=>x -> x -> Bool
coprime x y | solver x y == 1 = True
			| otherwise = False

			
checkLower:: String -> Bool
checkLower [] = True
checkLower xs | isLower (head xs) == True = checkLower (tail xs)
			  | otherwise = False
			
checkString:: String -> String-> Bool
checkString xs ys = if last xs == 'h' && last ys == 'h' then if length(filter(\x -> x == 'h') xs) == 1 && length(filter(\y -> y == 'h') ys) == 1 then if filter(\x -> x /= 'a') xs == "h" &&  filter(\y -> y /= 'a') ys == "h" then True else False else False else False
			
seeDoctor:: String ->  String ->  Bool 
seeDoctor [] [] = True
seeDoctor [] ys = if last ys == 'h' then if filter(\y -> y /= 'a') ys == "h" then True else False else False
seeDoctor xs [] = False
seeDoctor xs ys = if checkLower xs == True then if checkLower ys == True  then if checkString xs ys == True then if length xs <= length ys then True else False else False else False else False


waterGate :: Int -> Int
waterGate x = gatesOpen(gatesNest x  0 (replicate x False))


swapGates:: Bool -> Bool
swapGates x | x == True = False
			| otherwise = True
			
gatesOpen:: (Num x)=>[Bool] -> x
gatesOpen [] = 0
gatesOpen (x:xs) | x == True = 1 + gatesOpen xs
				 | otherwise = gatesOpen xs
				 
changeGates:: Int -> Int -> [Bool] -> [Bool]
changeGates 0 i xs = xs
changeGates n i (x:[]) | n == i = [swapGates x]
					   | otherwise = [x]
changeGates n i (x:xs) | n == i = (swapGates x:changeGates n 1 xs)
					   | otherwise = (x:changeGates n (i + 1) xs)
					   
gatesNest:: Int -> Int -> [Bool] -> [Bool]
gatesNest n i xs     | n == i = xs
					 | otherwise = gatesNest n (i + 1) (changeGates(n - i) 1 xs)
					 
					 
goldbachNum :: Int
goldbachNum = multOperation 5701 1 1

--findNum:: Int->Bool
--findNum x | isOdd x == True = multOperation x y 1
--		  | otherwise = findNum (x + 1)
		  
--findFactor:: Int -> Int -> Int
--findFactor x y = if isFactor x y then multOperation x y 1 else findNum (x + 1)

multOperation:: Int -> Int -> Int -> Int
multOperation x y z = if (y + (2 * z^2)) == x then multOperation (x + 2) 1 1 else if (y + (2 * z^2)) > x then if (x < y) then x else multOperation (x + 2) 1 1 else if (x > y) && nextPrime y < x then multOperationZ x (nextPrime y) z else multOperation (x + 2) 1 1

multOperationZ:: Int -> Int -> Int -> Int
multOperationZ x y z = if (y + (2 * z^2)) == x then multOperation (x + 2) 1 1 else if ((2 * z^2) < x) && ((2 * (z + 1)^2)) < x then multOperationZ x y (z + 1) else if nextPrime y > x then x else multOperation x y 1 

nextPrime:: Int -> Int
nextPrime x | isPrime (x + 1) == True = (x + 1)
			| otherwise = nextPrime (x + 1)
			
isPrime:: Int->Bool
isPrime k = null [ x | x <- [2..k - 1], k `mod`x  == 0]

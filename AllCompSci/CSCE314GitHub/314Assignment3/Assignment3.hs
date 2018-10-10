crt :: [(Integer, Integer)] -> (Integer, Integer)
crt [] = (0,0)
crt [(0,0)] = (0,0)
crt [(x,y)] = (x,y)
crt xs = (firstTuple xs, secondTuple xs)

lcd:: [Integer] -> [Integer] -> Integer
lcd [][] = 0
lcd (x:xs) ys = if x `elem` ys then x else lcd xs ys

secondTuple:: (Num b) => [(bl, b)] -> b
secondTuple [] = 1
secondTuple ((_,x):xs) = x * secondTuple xs

makeList:: (Integer, Integer) -> Integer -> [Integer]
makeList (_,0) b = [0]
makeList (a,y) b = [x | x <- [1..b], (x `mod` y) == a] 

firstTuple:: [(Integer, Integer)] -> Integer
firstTuple (x:y:ys) | null ys = lcd (makeList x ((snd x) * (snd y)))(makeList y ((snd x) * (snd y)))
					| otherwise = firstTuple (((lcd (makeList x ((snd x) * (snd y))) (makeList y ((snd x) * (snd y))), ((snd x) * (snd y))) :ys))
					
kcomposite:: Int -> [Int]
kcomposite n = klength n 0

factor:: Int -> [Int]
factor n = [x| x <- [1..n], (n `mod` x) == 0]

klength:: Int -> Int -> [Int]
klength x z        | length(factor z) == (x + 2) = z:klength x (z + 1)
				   | otherwise = klength x (z + 1)

anagramEncode :: [Char] -> [Char]
anagramEncode [_] = []
anagramEncode xs = readChar(restructureChar(addX xs) (last (init(factor(length(addX xs))))))

addX:: [Char] -> [Char]
addX xs | length(factor (length xs)) == 4 = xs
	    | otherwise                       = addX (xs++"X")
		
restructureChar:: [Char] -> Int -> [[Char]]
restructureChar [] _ = []
restructureChar xs y = (take y xs):(restructureChar(drop y xs) y)

readChar::[[Char]] -> [Char]
readChar ([]:_) = []
readChar [] = ""
readChar xs = (map head xs) ++ readChar(map (drop 1) xs)

anagramDecode :: [Char] -> [Char]
anagramDecode [] = []
anagramDecode xs = removeX(readChar(restructureChar xs (head(tail(factor(length xs))))))

removeX :: [Char] -> [Char]
removeX [] = []
removeX xs | last xs == 'X' = removeX(reverse (drop 1 (reverse xs)))
		   | otherwise = xs
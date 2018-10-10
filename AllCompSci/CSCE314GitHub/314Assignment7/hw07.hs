-- Assignment 7, CSCE-314
 
module Main where
 
import Prelude hiding (lookup)
 
import Test.HUnit
import System.Exit
 
-- AST definition for W
data WValue = VInt Int 
            | VBool Bool 
			| VMarker
              deriving (Eq, Show)
 
data WExp = Val WValue
 
          | Var String
 
          | Plus WExp WExp
          | Minus WExp WExp
          | Multiplies WExp WExp
          | Divides WExp WExp
 
          | Equals WExp WExp
          | NotEqual WExp WExp
          | Less WExp WExp
          | Greater WExp WExp
          | LessOrEqual WExp WExp
          | GreaterOrEqual WExp WExp
 
          | And WExp WExp
          | Or WExp WExp
          | Not WExp
 
data WStmt = Empty
           | VarDecl String WExp
           | Assign String WExp
           | If WExp WStmt WStmt
           | While WExp WStmt
           | Block [WStmt]
 
type Memory = [(String, WValue)]
marker = ("|", VMarker)
isMarker (x, _) = x == "|"
 
-- eval function
--All eval functions are simply derived from their normal counterparts but with the addtion of the asInt and the recursion capabilities of adding eval within the
--function declaration. All eval functions return in either VBool or VInt
eval :: WExp -> Memory -> WValue
eval (Val a) b = a
eval (Var a) b | lookup a b == Nothing = error "Unknown Variable"
			   | otherwise = eval (Val(fromJust(lookup a b))) b
eval (Plus a b) c = VInt ((asInt (eval a c)) + (asInt (eval b c)))
eval (Plus _ _) c = error "Plus error, no varaible"
eval (Minus a b) c = VInt ((asInt (eval a c)) - (asInt (eval b c)))
eval (Minus _ _) c = error "Minus error, no varaible"
eval (Multiplies a b) c = VInt ((asInt (eval a c)) * (asInt (eval b c)))
eval (Multiplies _ _) c = error "Multiplies error, no varaible"
eval (Divides a b) c = VInt ((asInt (eval a c)) `div` (asInt (eval b c)))
eval (Divides _ _) c = error "Divides error, no varaible"
eval (Equals a b) c = VBool ((asInt (eval a c)) == (asInt (eval b c)))
eval (Equals _ _) c = error "Equals error, no varaible"
eval (NotEqual a b) c = VBool ((asInt (eval a c)) /= (asInt (eval b c)))
eval (NotEqual _ _) c = error "Not Equal error, no varaible"
eval (Less a b) c = VBool ((asInt (eval a c)) < (asInt (eval b c)))
eval (Less _ _) c = error "Less error, no varaible"
eval (Greater a b) c = VBool ((asInt (eval a c)) > (asInt (eval b c)))
eval (Greater _ _) c = error "Greater error, no varaible"
eval (LessOrEqual a b) c = VBool ((asInt (eval a c)) <= (asInt (eval b c)))
eval (LessOrEqual _ _) c = error "Less or Equal error, no varaible"
eval (GreaterOrEqual a b) c = VBool ((asInt (eval a c)) >= (asInt (eval b c)))
eval (GreaterOrEqual _ _) c = error "Greater or Equal error, no varaible"
eval (And a b) c = VBool ((asBool (eval a c)) && (asBool (eval b c)))
eval (And _ _) c = error "And error, no varaible"
eval (Or a b) c = VBool ((asBool (eval a c)) || (asBool (eval b c)))
eval (Or _ _) c = error "Or error, no varaible"
eval (Not a) c = VBool (not (asBool (eval a c)))
eval (Not _) c = error "Not error, no varaible"
			   
-- exec function
exec :: WStmt -> Memory -> Memory
exec (Empty) c = c
--Adds variable to the stack if there is currently nothing there and adds the variable to the memory, this is necessary to use the assign function
exec (VarDecl a b) c | lookup a c == Nothing = (a, eval b c):c
					 | otherwise = error "Redeclaration"
			
--Check to see if a variable exists in memory c, if it does exist the helper function is executed and the variable is assigned within the memory
exec (Assign a b) c | lookup a c == Nothing = error "Place in memory does not exist"
					| otherwise = helper a (eval b c) c

--If the a variable is true, the b variable is executed and if the a variable is false, the c variable is executed
exec (If a b c) d | eval a d == VBool(True) = exec b d
				  | otherwise = exec c d

				  --Continue to run eval function until the a variable turns to false				  
exec (While a b) c | eval a c == VBool(True) = exec (While a b) (exec b c)
				   | eval a c == VBool(False) = c
				   | otherwise = error "While error"

--The foldr function is used to divide the "Block" using markers. Each exec function is evaluated in the correct order and markers are skipped when
--coming into contact
exec (Block xs) c = helper3 $ foldr exec (marker:c) (reverse xs)
						  
--fibonnacci program acts using the asInt and fromJust predefined functions. It simply uses the fibonnacciProgram (explained below)
--to lookup the nth fibonnacci digit in the fibonnacci chain
fibonnacci:: Int -> Int
fibonnacci a = asInt(fromJust (lookup "x" (exec fibonnacciProgram[("x", VInt(a))])))

-- example programs
factorial = 
  Block
  [
    VarDecl "acc" (Val (VInt 1)),
    While (Greater (Var "x") (Val (VInt 1)))
    (
      Block
      [
        Assign "acc" (Multiplies (Var "acc") (Var "x")),
        Assign "x" (Minus (Var "x") (Val (VInt 1)))         
      ]
    ),
    Assign "result" (Var "acc")
  ]
 
p1 = Block
     [
       VarDecl "x" (Val (VInt 0)),
       VarDecl "b" (Greater (Var "x") (Val (VInt 0))),
       If (Or (Var "b") (Not (GreaterOrEqual (Var "x") (Val (VInt 0)))))
         ( Block [ Assign "x" (Val (VInt 1)) ] )
         ( Block [ Assign "x" (Val (VInt 2)) ] )
     ]

--The fibonnacci program is derived from a standard c++ fibonnacci implementation. Three variables are kept and the first and second variables are added
--together in order to make up the third variable, then the variables are respectively shifted left one step, giving a the c variable, b the a variable and c
--is the new variable made up by adding the first two. The process is iterated x number of times	 
fibonnacciProgram = Block
	[
		VarDecl "Count" (Val (VInt 0)),
		VarDecl "a" (Val (VInt 0)),
		VarDecl "b" (Val (VInt 1)),
		VarDecl "c" (Val (VInt 0)),
		While(LessOrEqual (Var "Count") (Var "x"))
		(
			Block [
					Assign "c" (Plus (Var "a") (Var "b")),
					Assign "b" (Var "a"),
					Assign "a" (Var "c"),
					Assign "Count" (Plus (Var "Count") (Val (VInt (1))))
			
				  ]	
		),
		Assign "x" (Var "c")
	
	
	]
 
-- some useful helper functions
lookup s [] = Nothing
lookup s ((k,v):xs) | s == k = Just v
                    | otherwise = lookup s xs
 
asInt (VInt v) = v
asInt x = error $ "Expected a number, got " ++ show x
 
asBool (VBool v) = v
asBool x = error $ "Expected a boolean, got " ++ show x
 
fromJust (Just v) = v
fromJust Nothing = error "Expected a value in Maybe, but got Nothing"

--Used to check to see if the varaible a is equivalent to the variable d in memory. If so the variable is added in memory at that position. If the varaible
--is not already available in memory it is added to the front of the memory stack
helper _ _ [] = []
helper a b (x@(d,_):xs)| a == d = if helper2 b x then (d,b):xs else error "Assign Error"			
					   | otherwise = x:helper a b xs

--Used in part with the helper function to check to see if the types of the variable and the memory variable are the same
helper2 b@(VInt _) x@(_,VInt _) = True 
helper2 b@(VBool _) x@(_,VBool _) = True
helper2 _ _ = False

--Used in part with the Block function to check to see if the variable at the given point in memory is a marker, if so the rest of the string is returned					   
helper3 [] = []
helper3 (x:xs) | isMarker x = xs
			   | otherwise = helper3 xs
 
-- unit tests
myTestList =
 
  TestList [
    test $ assertEqual "p1 test" [] (exec p1 []),
 
    let res = lookup "result" (exec factorial [("result", VInt(-1)), ("x", VInt 10)])
    in test $ assertBool "factorial of 10" (3628800 == asInt (fromJust res))
    ]    
 
-- main: run the unit tests  
main = do c <- runTestTT myTestList
          putStrLn $ show c
          let errs = errors c
              fails = failures c
          if (errs + fails /= 0) then exitFailure else return ()
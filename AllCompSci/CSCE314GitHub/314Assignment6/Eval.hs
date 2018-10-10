module Main where

data E = IntLit Int
       | BoolLit Bool
       | Plus E E
       | Minus E E
       | Multiplies E E
       | Exponentiate E E
       | Equals E E
         deriving (Eq, Show)
		 
--Go through each of the functions and define according to the function definition. If types are not equivalent, then
--throw and error. If providing an evaluation on evaluation, then define function to accept functions as well.
eval :: E -> E
eval e = case e of
		IntLit b -> IntLit b
		BoolLit b -> BoolLit b
		Plus (BoolLit b) _ -> error "Non-exhaustive patterns"
		Plus _ (BoolLit b) -> error "Non-exhaustive patterns"
		Plus (IntLit b) (IntLit c) -> IntLit(b + c)
		Plus e1 e2 -> eval (Plus (eval e1) (eval e2))
		Minus (BoolLit b) _ -> error "Non-exhaustive patterns"
		Minus _ (BoolLit b) -> error "Non-exhaustive patterns"
		Minus (IntLit b) (IntLit c) -> IntLit(b - c)
		Minus e1 e2 -> eval (Minus (eval e1) (eval e2))
		Multiplies (BoolLit b) _ -> error "Non-exhaustive patterns"
		Multiplies _ (BoolLit b) -> error "Non-exhaustive patterns"
		Multiplies (IntLit b) (IntLit c) -> IntLit(b * c)
		Multiplies e1 e2 -> eval (Multiplies (eval e1) (eval e2))
		Exponentiate (BoolLit b) _ -> error "Non-exhaustive patterns"
		Exponentiate _ (BoolLit b) -> error "Non-exhaustive patterns"
		Exponentiate (IntLit b) (IntLit c) -> IntLit(b ^ c)
		Exponentiate e1 e2 -> eval (Exponentiate (eval e1) (eval e2))
		Equals (BoolLit b) (BoolLit c) -> BoolLit(b == c)
		Equals (IntLit b) (BoolLit c) -> error "Non-exhaustive patterns"
		Equals (BoolLit b) (IntLit c) -> error "Non-exhaustive patterns"
		Equals (IntLit b) (IntLit c) -> BoolLit(b == c)
		Equals e1 e2 -> eval (Equals (eval e1) (eval e2))
		
	

--Change all the normal operations to calculations using exponents and logs, therefore multiplying is equivalent to adding, Exponentiating is equivalent to
--multiplying etc..		
log2Sim :: E -> E
log2Sim e = case e of
			IntLit b -> IntLit(log2Calc b)
			BoolLit b -> BoolLit b
			Multiplies e1 e2 -> Plus (log2Sim e1) (log2Sim e2)
			Exponentiate e1 e2 -> Multiplies e1 (log2Sim e2)
			Equals e1 e2 -> Equals (log2Sim e1) (log2Sim e2)
			
--TO calculate the log2 of a variable, check to see if it is divisible by log2 by checking to see if the length of 2^n is one, then using that number if
--it is valid.
log2Calc :: Int -> Int
log2Calc 1 = 0
log2Calc b = if(length([n|n <- [1..b], 2^n == b]) == 1) then (head([n|n <- [1..b], 2^n == b])) else error "Non-exhaustive patterns"				


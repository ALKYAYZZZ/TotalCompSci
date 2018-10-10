module Main where

data Tree a b = Branch b (Tree a b) (Tree a b)
              | Leaf a

--When an instance of Tree is thrown, leafs are indented by two and shown to the console whereas branches are indented by one and a newline
--is created making a stairstep pattern.
instance (Show a, Show b) => Show(Tree a b)where
	show (Leaf a) = (pushforward 2) ++ show a
	show (Branch a b c) = show a ++ "\n" ++ (pushforward 1) ++ show b ++ "\n"  ++ (pushforward 1) ++ show c

--To Indent we use a recursive function to push the text forward using a int to string conversion.	
pushforward:: Int -> String
pushforward 0 = []
pushforward a = " " ++ pushforward (a - 1)

--Inorder outputs the branch "in order", so the second function evaluates the branch in between the instances of in order 
--evaluating the trees b and c. This recursively solves the tree using the in order
--notation. Leaf requires no recursion so the answer is directly output using the first function
inorder   :: (a -> c) -> (b -> c) -> Tree a b -> [c]
inorder f g (Leaf a) = [f a]
inorder f g (Branch a b c) = inorder f g b ++ [g a] ++ inorder f g c
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

--Postorder evaulates the function by concatinating the branch last, so the instances of postorder recursively
--Evaluating the trees are before the branch is output
postorder   :: (a -> c) -> (b -> c) -> Tree a b -> [c]
postorder f g (Leaf a) = [f a]
postorder f g (Branch a b c) = postorder f g b ++ postorder f g c ++ [g a]
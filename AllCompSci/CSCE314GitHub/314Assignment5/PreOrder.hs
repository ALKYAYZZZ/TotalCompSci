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

--Preorder evaulates the function by concatinating the branch first. The instance where the second function
--outputs the branch is listed first whereas the recursive funtions of postorder that evaluates the trees are
--concatinated after
preorder   :: (a -> c) -> (b -> c) -> Tree a b -> [c]
preorder f g (Leaf a) = [f a]
preorder f g (Branch a b c) = [g a] ++ preorder f g b ++ preorder f g c
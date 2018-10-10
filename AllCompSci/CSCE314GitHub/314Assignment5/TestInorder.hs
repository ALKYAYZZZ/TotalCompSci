module TestInorder where

import Test.HUnit
import System.Exit
import Main

tree1 = Branch "A" 
           (Branch "B" 
              (Leaf (1::Int)) 
              (Leaf (2::Int))) 
           (Leaf (3::Int))

tree2 = Branch 1 
           (Branch 2
              (Leaf "A") 
              (Leaf "B")) 
           (Leaf "C")

tree3 = Leaf "A"

tree4 = Leaf (1::Int)

tree5 = Branch "A" 
           (Branch "B" 
              ((Branch "C" 
              (Leaf (1::Int)) 
              (Leaf (2::Int)))) 
              (Leaf (3::Int))) 
           (Branch "D" 
              (Leaf (4::Int)) 
              (Leaf (5::Int)))

tree6 = Branch 1 
           (Branch 2
              (Leaf "A") 
              (Leaf "B"))
           (Branch 3
              (Leaf "C") 
              (Branch 4
              (Leaf "D") 
              (Leaf "E")))


myTestList = 
  TestList [ 
      "An elementary case failed! (H3)"  ~: ((inorder show id tree1) == ["1","B","2","A","3"]) ~=? True
    , "An elementary case failed! (H3)"  ~: ((inorder id show tree2) == ["A","2","B","1","C"]) ~=? True
    , "A Tree with only a leaf node failed!" ~: ((inorder id id tree3) == ["A"]) ~=? True
    , "A Tree with only a leaf node failed!" ~: ((inorder show id tree4) == ["1"])~=? True
    , "An elementary case failed! (H4)"  ~: ((inorder show id tree5) == ["1","C","2","B","3","A","4","D","5"]) ~=? True
    , "An elementary case failed! (H4)"  ~: ((inorder id show tree6) == ["A","2","B","1","C","3","D","4","E"]) ~=? True
    ]

main = do c <- runTestTT myTestList
          putStrLn $ show c
          let errs = errors c
              fails = failures c
          exitWith (codeGet errs fails)
          
codeGet errs fails
 | fails > 0       = ExitFailure 2
 | errs > 0        = ExitFailure 1
 | otherwise       = ExitSuccess

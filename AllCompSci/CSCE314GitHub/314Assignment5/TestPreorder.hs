module TestPreorder where

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
      "An elementary case failed! (H3)"  ~: ((preorder show id tree1) == ["A","B","1","2","3"]) ~=? True
    , "An elementary case failed! (H3)"  ~: ((preorder id show tree2) == ["1","2","A","B","C"]) ~=? True
    , "A Tree with only a leaf node failed！" ~: ((preorder id id tree3) == ["A"]) ~=? True
    , "A Tree with only a leaf node failed！" ~: ((preorder show id tree4) == ["1"])~=? True
    , "An elementary case failed! (H4)"  ~: ((preorder show id tree5) == ["A","B","C","1","2","3","D","4","5"]) ~=? True
    , "An elementary case failed! (H4)"  ~: ((preorder id show tree6) == ["1","2","A","B","3","C","4","D","E"]) ~=? True
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

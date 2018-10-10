module TestPostorder where

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
      "An elementary case failed! (H3)"  ~: ((postorder show id tree1) == ["1","2","B","3","A"]) ~=? True
    , "An elementary case failed! (H3)"  ~: ((postorder id show tree2) == ["A","B","2","C","1"]) ~=? True
    , "A Tree with only a leaf node failed!" ~: ((postorder id id tree3) == ["A"]) ~=? True
    , "A Tree with only a leaf node failed!" ~: ((postorder show id tree4) == ["1"])~=? True
    , "An elementary case failed! (H4)"  ~: ((postorder show id tree5) == ["1","2","C","3","B","4","5","D","A"]) ~=? True
    , "An elementary case failed! (H4)"  ~: ((postorder id show tree6) == ["A","B","2","C","D","E","4","3","1"]) ~=? True
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

module TestIsElement where

import Test.HUnit
import System.Exit
import Main

myTestList = 
  TestList [ 
      "[isElement: Does your code work for char lists (a case where the element is inside the list failed)?]"   ~: (isElement 'c' "abcd") ~=? True
    , "[isElement: Does your code work for char lists (a case where the element is first in the list failed)?]"   ~: (isElement 'a' "abcd") ~=? True
    , "[isElement: Does your code work for char lists (a case where the element is last in the list failed)?]"   ~: (isElement 'd' "abcd") ~=? True
    , "[isElement: Does your code work for char lists (a case where the element is not inside the list failed)?]"  ~: (isElement 'e' "abcd") ~=? False
    , "[isElement: Does your code work for empty lists?]"    ~: (isElement 1 []) ~=? False
    , "[isElement: An elementary case failed!]"   ~: (isElement 2 [-5..10]) ~=? True
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

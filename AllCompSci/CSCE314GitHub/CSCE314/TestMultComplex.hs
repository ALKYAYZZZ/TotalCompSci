module TestMultComplex where

import Test.HUnit
import System.Exit
import Main

myTestList = 
  TestList [ 
      "[multComplex: An elementary case failed!]"    ~: multComplex (1,2) (2,3) ~=? (-4,7)
    , "[multComplex: Does your code work for the case containing negative number?]"     ~: multComplex (-1,2) (2,3) ~=? (-8,1)
    , "[multComplex: Does your code work for the case containing non-integers]"     ~: multComplex (1.5,2) (2,-3) ~=? (9,-0.5)
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

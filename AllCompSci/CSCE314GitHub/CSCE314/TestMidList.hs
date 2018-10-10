module TestMidList where

import Test.HUnit
import System.Exit
import Main

myTestList = 
  TestList [ 
      "[midList: An elementary case failed!]" ~: midList [1..10] ~=? [2..9]
    , "[midList: Does your code work for the case with no element in the list?]"    ~: midList ([] :: [Int]) ~=? []
    , "[midList: Does your code work for the case with one element in the list?]"    ~: midList [1] ~=? []
    , "[midList: Does your code work for the case with two elements in the list?]"     ~: midList [1,2] ~=? []
    , "[midList: Does your code work for the case with non-integer elements in the list?]"     ~: midList [1.2, 2.3, 3.4] ~=? [2.3]
    , "[midList: Does your code work for the case with non-number elements in the list?]"     ~: midList ['a','b','c'] ~=? ['b']
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

module TestCountdownList where

import Test.HUnit
import System.Exit
import Main

myTestList = 
  TestList [ 
      "[countdownList: An elementary case failed!]"    ~: countdownList 1 5 ~=? [5,4,3,2,1]
    , "[countdownList: Does your code check the requirement that the 2nd argument should be larger than the 1st?]" ~: (countdownList 1 1 == [1])|| (countdownList 1 1 == [])~=? False
    , "[countdownList: Does your code work for the case with two consecutive integers?]" ~: countdownList 1 2 ~=? [2,1]
    , "[countdownList: Does your code work for the case with one negative integer?]"     ~: countdownList (-1) 2 ~=? [2,1,0,-1]
    , "[countdownList: Does your code work for the case with two negative integers?]"     ~: countdownList (-2) (-1) ~=? [-1,-2]
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

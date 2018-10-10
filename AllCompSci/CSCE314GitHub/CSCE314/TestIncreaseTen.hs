module TestIncreaseTen where

import Test.HUnit
import System.Exit
import Main

myTestList = 
  TestList [ 
      "[increaseTen：An elementary case failed!]"    ~: increaseTen 0 ~=? 10
    , "[increaseTen: Are negative case handled?]"   ~: increaseTen (-1) ~=? 9
    , "[increaseTen: Does your code work for non-integers?]"    ~: increaseTen 1.1 ~=? 11.1
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

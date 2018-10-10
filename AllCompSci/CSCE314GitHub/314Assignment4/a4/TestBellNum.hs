module TestBellNum where

import Test.HUnit
import System.Exit
import Main
import TestHelper

myTestList = 
  TestList [ 
      "An elementary case failed! (B0)"  ~: bellNum 0 ~=? 1
    , "An elementary case failed! (B1)"  ~: bellNum 1 ~=? 1
    , "An elementary case failed! (B2)"  ~: bellNum 2 ~=? 2
    , "An elementary case failed! (B3)"  ~: bellNum 3 ~=? 5
    , "An elementary case failed! (B4)"  ~: bellNum 4 ~=? 15
    , "An elementary case failed! (B5)"  ~: bellNum 5 ~=? 52
    , "An elementary case failed! (B6)"  ~: bellNum 6 ~=? 203
    , "An elementary case failed! (B7)"  ~: bellNum 7 ~=? 877
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

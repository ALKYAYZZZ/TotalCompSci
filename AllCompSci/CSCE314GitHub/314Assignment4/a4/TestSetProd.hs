module TestSetProd where

import Test.HUnit
import System.Exit
import Main
import TestHelper

myTestList = 
  TestList [ 
      "An elementary case failed! (The input is an integer list and a char list)"  ~: helpersetEqual (setProd [1,2,3] ['a','b']) [(1,'a'),(1,'b'),(2,'a'),(2,'b'),(3,'a'),(3,'b')] ~=? True
    , "An elementary case failed! (The input is an integer list and a string list)" ~: helpersetEqual (setProd [1,2] ["abc", "def"]) [(1,"abc"),(2,"abc"),(1,"def"),(2,"def")] ~=? True
    , "An elementary case failed! (The input is a char list and an integer list)" ~: helpersetEqual (setProd ['a','b'] [1,2] ) [('a',1),('a',2),('b',1),('b',2)] ~=? True
    , "An elementary case failed! (The input is a char list with one element and an integer list)" ~: helpersetEqual (setProd ['a'] [1,2] ) [('a',1),('a',2)] ~=? True
    , "An elementary case failed! (The input is a char list with one element and an integer list with one element)" ~: helpersetEqual (setProd ['a'] [1] ) [('a',1)] ~=? True
    , "The case taking an empty integer list and a char list as input failed!" ~: helpersetEqual (setProd ([]::[Int]) ['a','b'] ) [] ~=? True
    , "The case taking two empty integer lists as input failed!" ~: helpersetEqual (setProd ([]::[Int]) ([]::[Int])) [] ~=? True
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

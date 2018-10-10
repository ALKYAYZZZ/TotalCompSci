module TestMkSet where

import Test.HUnit
import System.Exit
import Main
import TestHelper

myTestList = 
  TestList [ 
      "An elementary case failed! (The input is a string with duplicate characters)"  ~: helpersetEqual (mkSet "abcdaab") "abcd"~=? True
    , "An elementary case failed! (The input is a string with no duplicate characters)"  ~: helpersetEqual (mkSet "abc") "bac"~=? True
    , "An elementary case failed! (The input is an empty string)"  ~: helpersetEqual (mkSet "") ""~=? True
    , "An elementary case failed! (The input is an integer list with duplicate integers)"  ~: helpersetEqual (mkSet [0,1,1,2,3,3]) [0,1,2,3]~=? True
    , "An elementary case failed! (The input is an integer list with no duplicate integers)"  ~: helpersetEqual (mkSet [0,1,2,3,4]) [0,1,2,3,4]~=? True
    , "An elementary case failed! (The input is an empty integer list)"  ~: helpersetEqual (mkSet []::[Int]) ([]::[Int])~=? True
    , "An elementary case failed! (The input is a string list with duplicate strings)"  ~: helpersetEqual (mkSet ["haskell","java","c++","python","haskell","java"]) ["haskell","java","c++","python"]~=? True
    , "An elementary case failed! (The input is a string list with no duplicate strings)"  ~: helpersetEqual (mkSet ["haskell","java","c++","python"]) ["haskell","java","c++","python"] ~=? True
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

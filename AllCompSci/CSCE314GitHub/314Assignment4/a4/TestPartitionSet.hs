module TestPartitionSet where

import Test.HUnit
import System.Exit
import Main
import TestHelper

myTestList = 
  TestList [ 
      "An elementary case failed! (The input is an integer set with two elements)"  ~: setEqualCMY (partitionSet [1,2])([[[1,2]],[[1],[2]]]) ~=? True
    , "An elementary case failed! (The input is an integer set with three elements)" ~: setEqualCMY (partitionSet [1,2,3]) [[[2],[3,1]],[[2,1],[3]],[[3,2,1]],[[1],[3,2]],[[1],[2],[3]]] ~=? True
    , "An elementary case failed! (The input is an integer set with one element)"  ~: setEqualCMY (partitionSet [1]) [[[1]]] ~=? True
    , "An elementary case failed! (The input is an integer set with no element)"  ~: setEqualCMY (partitionSet ([]::[Int])) [[[]::[Int]]] ~=? True
    , "An elementary case failed! (The input is a char set with two elements)"  ~: setEqualCMY (partitionSet "ab") [["ba"],["a","b"]] ~=? True
    , "An elementary case failed! (The input is a char set with one element)"  ~: setEqualCMY (partitionSet "a") [["a"]] ~=? True
    , "An elementary case failed! (The input is a string set with three elements)"  ~: setEqualCMY (partitionSet ["red", "green", "blue"]) [[["green","red"],["blue"]],[["blue","red"],["green"]],[["blue","green","red"]],[["red"],["blue","green"]],[["red"],["green"],["blue"]]] ~=? True
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

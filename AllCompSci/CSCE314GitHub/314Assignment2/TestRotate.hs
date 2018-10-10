module TestRotate where

import Test.HUnit
import System.Exit
import Main

myTestList = 
  TestList [ 
      "[rotateList : An elementary case has failed!]"  ~: rotate ['a','b','c','d','e','f','g','h'] 3 ~=? "defghabc"
    , "[rotateList : An elementary case has failed!]" ~: rotate [1,2,3,4,5] (2)  ~=? [3,4,5,1,2]
    , "[rotateList : Your code does not work for empty strings!]" ~: rotate "" 2 ~=? ""
    , "[rotateList : Your code does not work when the rotation location is greater than the length of the string!]"  ~: rotate ['a','b','c','d','e','f','g','h'] 11 ~=? "defghabc"
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

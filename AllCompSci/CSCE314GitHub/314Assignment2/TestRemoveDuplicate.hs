module TestRemoveDuplicate where

import Test.HUnit
import System.Exit
import Main

myTestList = 
  TestList [ 
      "[removeDuplicate: An elementary case failed!]"    ~: removeDuplicate [1,1,1,3,3,3,5,7,8,8] ~=? [1,3,5,7,8]
    , "[removeDuplicate: Does your code work for the case with only one integer?]" ~: removeDuplicate [4] ~=? [4]
    , "[removeDuplicate: Does your code work for the case with negative integers?]"     ~: removeDuplicate [-5,-5,0,0,0,2] ~=? [-5,0,2]
    , "[removeDuplicate: Does your code work for the case with empty list?]"     ~: (removeDuplicate []::[Int]) ~=? []
    , "[removeDuplicate: Does your code work for strings?]" ~: removeDuplicate "aabbcc" ~=? "abc"
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

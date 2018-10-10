module TestDuplicate where

import Test.HUnit
import System.Exit
import Main

myTestList = 
  TestList [ 
      "[duplicateList: An elementary case failed!]"      ~: duplicate [1,2,3] ~=? [1,1,2,2,3,3]
    , "[duplicateList: An elementary case failed!]"      ~: duplicate [2,2] ~=? [2,2,2,2]
    , "[duplicateList: Does your code work for string?]"      ~: duplicate "abc" ~=? "aabbcc"
    , "[duplicateList: Does your code work for empty lists?]" ~: (duplicate []::[Int]) ~=? []
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

module TestUppercaseList where

import Test.HUnit
import System.Exit
import Main

myTestList = 
  TestList [ 
      "[uppercaseList: Does your code work for empty string?]"                 ~: uppercaseList "" ~=? []
    , "[uppercaseList: Your code failed to recognize upper case letter!]"     ~: uppercaseList "A" ~=? [(True,False,False)]
    , "[uppercaseList: Your code failed to recognize lower case letter!]"     ~: uppercaseList "a" ~=? [(False,True,False)]
    , "[uppercaseList: Your code failed to recognize number letter!]"         ~: uppercaseList "1" ~=? [(False,False,True)]
    , "[uppercaseList: Your code failed to recognize special letter!]"         ~: uppercaseList "@" ~=? [(False,False,False)]
    , "[uppercaseList: An elementary case failed!]"                          ~: uppercaseList "Ab1." ~=? [(True, False, False), (False, True, False), (False, False, True), (False, False, False)]
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

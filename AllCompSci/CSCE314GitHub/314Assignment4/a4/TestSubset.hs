module TestSubset where

import Test.HUnit
import System.Exit
import Main

myTestList = 
  TestList [
     "An elementary case failed! (True case, taking two strings as input)" ~: subset "db" "abcd"~=? True
    ,"An elementary case failed! (True case, taking two same strings as input)" ~: subset "abcd" "abcd"~=? True
    ,"An elementary case failed! (True case, taking two strings containing the same characters as input)" ~: subset "abcd" "dcab"~=? True
    ,"An elementary case failed! (True case, taking an empty string as the first input)" ~: subset "" "abcd"~=? True
    ,"An elementary case failed! (True case, taking two empty strings as input)" ~: subset "" ""~=? True
    ,"An elementary case failed! (True case, taking two integer lists as input)" ~: subset [1,2] [1,2,3]~=? True
    ,"An elementary case failed! (True case, taking two same integer lists as input)" ~: subset [1,2,3] [1,2,3]~=? True
    ,"An elementary case failed! (True case, taking two integer lists containing the same integers as input)" ~: subset [1,2,3] [2,1,3]~=? True
    ,"An elementary case failed! (True case, taking an empty integer list as the first input)" ~: subset ([]::[Int]) [1,2,3]~=? True
    ,"An elementary case failed! (True case, taking two empty integer lists as input)" ~: subset ([]::[Int]) ([]::[Int])~=? True
    ,"An elementary case failed! (False case, taking two strings as input)" ~: subset "abcd" "db"~=? False
    ,"An elementary case failed! (False case, taking an empty string as the second input)" ~: subset "abcd" ""~=? False
    ,"An elementary case failed! (False case, taking two integer lists as input)" ~: subset [1,2,3] [1,2]~=? False
    ,"An elementary case failed! (False case, taking an empty integer list as the second input)" ~: subset [1,2,3] ([]::[Int]) ~=? False
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

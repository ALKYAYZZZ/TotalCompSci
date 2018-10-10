module TestKcomposite where

import Test.HUnit
import System.Exit
import Main

myTestList = 
  TestList [ 
    "An elementary case failed! (2-composite)"  ~: (take 5 $ kcomposite 2) ~=? [6,8,10,14,15]
   ,"An elementary case failed! (0-composite)"  ~: (take 5 $ kcomposite 0) ~=? [1,2,3,5,7]
   ,"An elementary case failed! (1-composite)"  ~: (take 5 $ kcomposite 1) ~=? [4,9,25,49,121]
   ,"An elementary case failed! (3-composite)"  ~: (take 4 $ kcomposite 3) ~=? [16,81,625,2401]   -- It takes a long time to find the 5th 3-composite number
   ,"An elementary case failed! (10-composite)"  ~: (take 5 $ kcomposite 10) ~=?[60,72,84,90,96]
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

module TestCoprime where

import Test.HUnit
import System.Exit
import Main

myTestList = 
  TestList [ 
      "[coprime: An elementary case failed (True case)!]"  ~: coprime 3 5 ~=? True
    , "[coprime: An elementary case failed (False case)!]"   ~: coprime 15 5 ~=? False
    , "[coprime: Does your code work for 0 and a non-one integer?]"   ~: coprime 15 0 ~=? False
    , "[coprime: Does your code work for 0 and one?]" ~: coprime 1 0 ~=? True
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

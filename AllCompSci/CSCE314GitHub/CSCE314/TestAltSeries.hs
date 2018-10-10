module TestAltSeries where

import Test.HUnit
import System.Exit
import Main

myTestList = 
  TestList [ 
    "[altSeries: An elementary case failed!]" ~: altSeries [1,2,3] ~=? 2
    , "[altSeries: Does your code work for lists containing negative number?]" ~: altSeries [-1,2,3] ~=? 0
    , "[altSeries: Does your code work for lists with only one element?]" ~: altSeries [1] ~=? 1
    , "[altSeries: Does your code work for lists with two elements?]" ~: altSeries [1,2] ~=? (-1)
    , "[altSeries: Does your code work for lists containing non-integers?]" ~: (abs(altSeries [1.1, 2.1, 3.1, 4.1] + 2))< (1e-10) ~=? True
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

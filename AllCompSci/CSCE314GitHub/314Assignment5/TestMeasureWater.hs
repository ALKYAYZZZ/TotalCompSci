module TestMeasureWater where

import Test.HUnit
import System.Exit
import Main

myTestList = 
  TestList [ 
      "An elementary case failed! "  ~: measureWater 3 5 4 ~=? True
    , "An elementary case failed! " ~: measureWater 2 6 5 ~=? False
    , "An elementary case failed! (There is a jug with capacity 0)]"  ~: measureWater 9 0 3 ~=? False
    , "An elementary case failed! (There is a jug with capacity 0, and the water needed equals to the other jug's capacity)]"  ~: measureWater 9 0 9 ~=? True
    , "An elementary case failed! (The water needed exceeds the sum of the capacities of two jugs)]"  ~: measureWater 2 2 8 ~=? False
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

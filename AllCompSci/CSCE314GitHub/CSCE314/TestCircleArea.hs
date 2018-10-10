module TestCircleArea where

import Test.HUnit
import System.Exit
import Main

myTestList = 
  TestList [ 
      "[circleArea: An elementary case failed!]"    ~: abs(circleArea 1 - 3.141592653589793) < (1e-10) ~=? True
    , "[circleArea: A large radius case failed!]"    ~: abs(circleArea (1e10) - 3.1415926535897933e20) < (1e-10) ~=? True
    , "[circleArea: A tiny radius case failed!]"    ~: abs(circleArea (1e-10) - 3.1415926535897936e-20) < (1e-10) ~=? True    
    , "[circleArea: Does your code check negative radius?]"    ~: ((abs(circleArea (-1.1) - 3.8013271108436504) < (1e-10)) == True) ~=? False
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

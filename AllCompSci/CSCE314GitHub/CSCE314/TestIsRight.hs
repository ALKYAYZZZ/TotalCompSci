module TestIsRight where

import Test.HUnit
import System.Exit
import Main

myTestList = 
  TestList [ 
      "[isRight: An elementary true case failed!]"     ~: isRight 3 4 5 ~=? True
    , "[isRight: An elementary false case failed!]" ~: isRight 4 5 6 ~=? False
    , "[isRight: Does your code check the case with side length equals to 0?]" ~: (isRight 0 2 2 == True) ~=? False
    , "[isRight: Does your code check the case with side length that is negative value?]" ~: (isRight (-3) 4 5 == True) ~=? True
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

module TestWaterGate where

import Test.HUnit
import System.Exit
import Main

myTestList = 
  TestList [ 
    "[waterGate: An elementary case failed!]" ~: waterGate 0 ~=? 0
    , "[waterGate: An elementary case failed!]" ~: waterGate 1 ~=? 1
    , "[waterGate: An elementary case failed!]" ~: waterGate 5 ~=? 2
    , "[waterGate: An elementary case failed!]" ~: waterGate 16 ~=? 4
    , "[waterGate: An elementary case failed!]" ~: waterGate 39 ~=? 6
    , "[waterGate: Does your code work for large numbers?]" ~: waterGate 87290198 ~=? 9342
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

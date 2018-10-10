module TestCrt where

import Test.HUnit
import System.Exit
import Main

myTestList = 
  TestList [ 
      "When there is only one tuple in the input list, the output should be the tuple itself!"  ~: crt [(2, 7)] ~=? (2, 7)
    , "An elementary case failed!"  ~: crt [(2, 7), (0, 3)] ~=? (9, 21) 
    , "An elementary case failed!"  ~: crt [(0, 3), (1, 5)] ~=? (6, 15)
    , "An elementary case failed!"  ~: crt [(2, 7), (1, 5)] ~=? (16, 35)
    , "An elementary case failed!"  ~: crt [(2, 7), (0, 3), (1, 5)] ~=? (51, 105)
    , "An elementary case failed!"  ~: crt [(2, 7), (0, 3), (3, 11)] ~=? (135, 231) 
    , "An elementary case failed!"  ~: crt [(2, 7), (0, 3), (1, 5),(3, 11)] ~=? (366, 1155)
    , "Although 10 and 21 are not primes, they are coprime, so crt [(2,10),(3,21)] = (192,210)." ~: crt [(2,10),(3,21)] ~=? (192,210)
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

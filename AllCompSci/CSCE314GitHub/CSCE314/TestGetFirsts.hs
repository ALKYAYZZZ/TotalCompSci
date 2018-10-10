module TestGetFirsts where

import Test.HUnit
import System.Exit
import Main

myTestList = 
  TestList [ 
      "[getFirsts: An elementary case failed!]"                             ~: getFirsts [(1,2),(3,4)] ~=? [1,3]    
    , "[getFirsts: Does your code work for the list with no element?]"                     ~: getFirsts ([] :: [(Int,Int)]) ~=? []
    , "[getFirsts: Does your code work for the list with one element?]"                 ~: getFirsts [(1,2)] ~=? [1]
    , "[getFirsts: Does your code work for the list with non-number elements?]"         ~: getFirsts [('a','b'),('c','d')] ~=? ['a','c']
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

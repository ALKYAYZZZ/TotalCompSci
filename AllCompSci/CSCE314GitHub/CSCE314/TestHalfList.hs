module TestHalfList where

import Test.HUnit
import System.Exit
import Main

myTestList = 
  TestList [ 
      "[halfList: An elementary case failed!]"                                 ~: halfList [1,2,3] ~=? [1,3]
    , "[halfList: Does your code work for empty list?]"                      ~: halfList ([] :: [Int]) ~=? []
    , "[halfList: Does your code work for lists with one element?]"                 ~: halfList [1] ~=? [1]
    , "[halfList: Does your code work for lists with non-integer elements?]"         ~: halfList ['a','b','c','d'] ~=? ['a','c']
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

module TestFlatten where

import Test.HUnit
import System.Exit
import Main

myTestList = 
  TestList [ 
      "[flatten: An elementary case failed!]"    ~: flatten [[1,2],[3,4],[5,6]] ~=? [1,2,3,4,5,6]
    , "[flatten: Does your code work for lists containing one empty list?]"   ~: flatten [[1], [2, 3, 4], [], [5, 6]] ~=? [1, 2, 3, 4, 5, 6]
    , "[flatten: Does your code work for lists containing empty lists?]"    ~: flatten [[]::[Int], [], []] ~=? []
    , "[flatten: Does your code work for strings (strings are lists)?]"   ~: flatten ["Howdy", " all", ", how are you?"] ~=? "Howdy all, how are you?"
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

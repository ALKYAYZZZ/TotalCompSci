module TestMyReverse where

import Test.HUnit
import System.Exit
import Main

myTestList = 
  TestList [ 
      "[myReverse: An elementary case failed!]"    ~: myReverse [1,2,3,4] ~=? [4,3,2,1]
    , "[myReverse: Reversing a string failed!]"     ~: myReverse "A man, a plan, a canal, panama!" ~=? "!amanap ,lanac a ,nalp a ,nam A"
    , "[myReverse: Your reverse function failed on an empty string!]" ~: myReverse "" ~=? ""
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

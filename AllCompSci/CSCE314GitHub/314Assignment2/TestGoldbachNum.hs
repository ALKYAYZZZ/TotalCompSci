module TestGoldbachnum where

import Test.HUnit
import System.Exit
import Main

myTestList = 
  TestList [ 
      "[goldbachNum: Wrong Number]"                 ~: goldbachNum  ~=? 5777
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

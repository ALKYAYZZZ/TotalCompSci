module TestDouble where

import Test.HUnit
import System.Exit
import Main

myTestList = 
  TestList [ 
           "[double: An elementary case failed!]"  ~: double 1 ~=? 2
           ,"[double: An elementary case failed!]" ~: double 0 ~=? 0
           ,"[double: Do floating point numbers work?]"  ~: double 33.3 ~=? 66.6
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

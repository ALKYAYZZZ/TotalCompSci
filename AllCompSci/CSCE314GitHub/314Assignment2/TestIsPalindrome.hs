module TestIsPalindrome where

import Test.HUnit
import System.Exit
import Main

myTestList = 
  TestList [ 
      "[isPalindrome: An elementary case failed!]"    ~: isPalindrome "madamimadam" ~=? True
    , "[isPalindrome: An elementary case failed!]" ~: isPalindrome "race a car" ~=? False
    , "[isPalindrome: An elementary case failed!]"     ~: isPalindrome "A man, a plan, a canal: Panama" ~=? False
    , "[isPalindrome: An elementary case failed!]"     ~: isPalindrome "0owlwo0" ~=? True
    , "[isPalindrome: Does your code work for the empty string?]" ~: isPalindrome "" ~=? True
    , "[isPalindrome: Does your code work for a string with a single character?]" ~: isPalindrome "x" ~=? True
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

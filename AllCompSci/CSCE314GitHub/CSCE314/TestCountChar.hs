module TestCountChar where

import Test.HUnit
import System.Exit
import Main

myTestList = 
  TestList [ 
        "[countChar: An elementary case failed!]"         ~: countChar 'l' "hellofadaf?fasdfa?%^89@fadsf" ~=? 2
    , "[countChar: An elementary case failed!]"         ~: countChar 'a' "hel#l$oghfdgfhgsfgsdfgfj+2#" ~=? 0
    , "[countChar: Does your code work for the case with empty string as the second argument?]"    ~: countChar 'a' "" ~=? 0
    , "[countChar: Does your code work for the case containing special char?]"     ~: countChar '\n' "\nrtrrgr\nfadfas#4$%fadaf" ~=? 2
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

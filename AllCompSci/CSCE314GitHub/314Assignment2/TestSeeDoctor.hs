module TestSeeDoctor where

import Test.HUnit
import System.Exit
import Main

myTestList = 
  TestList [ 
      "[seeDoctor: An elementary case failed (True case)!]"    ~: seeDoctor "aah" "aaaah" ~=? True
    , "[seeDoctor: An elementary case failed (False case)!]"    ~: seeDoctor "aaah" "ah" ~=? False
    , "[seeDoctor: Do you consider the case that the doctor requires you say nothing?]" ~: seeDoctor "" "aaah" ~=? True
    , "[seeDoctor: Do you consider the case that you can say nothing?]"   ~: seeDoctor "aah" "" ~=? False
    , "[seeDoctor: Do you condiser the case that the doctor requires nothing and you say nothing?]"  ~: seeDoctor "" "" ~=? True
    , "[seeDoctor: Do you consider the case that the doctor just needs to hear an 'h' ?]" ~: seeDoctor "h" "aah" ~=? True
    , "[seeDoctor: Do you consider the case that you can just say an 'h' ?]" ~: seeDoctor "aah" "h" ~=? False
    , "[seeDoctor: Do you consider the case that the doctor needs an 'h' while you can say nothing?]" ~: seeDoctor "h" "" ~=? False
    , "[seeDoctor: Do you consider the case that the doctor needs nothing and you can only say an 'h'?]" ~: seeDoctor "" "h" ~=? True
    , "[seeDoctor: Do you consider the case that the doctor needs an 'h' and you only say an 'h'?]" ~: seeDoctor "h" "h" ~=? True
    --- check the cases that the input strings do not meet the requirements
    , "[seeDoctor: Do you consider the case that the first input string does not end with an 'h'?]" ~: seeDoctor "a" "" ~=? False
    , "[seeDoctor: Do you consider the case that the second input string does not end with an 'h'?]" ~: seeDoctor "" "a" ~=? False
    , "[seeDoctor: Do you consider the case that both input strings do not end with an 'h'?]" ~: seeDoctor "a" "a" ~=? False
    , "[seeDoctor: Do you consider the case that the first input string contains characters other than 'a' and 'h'?]" ~: seeDoctor "x" "" ~=? False
    , "[seeDoctor: Do you consider the case that the first input string contains characters other than 'a' and 'h'?]" ~: seeDoctor "aaaxh" "aah" ~=? False
    , "[seeDoctor: Do you consider the case that the first input string contains characters other than 'a' and 'h'?]" ~: seeDoctor "aaahx" "aah" ~=? False
    , "[seeDoctor: Do you consider the case that the second input string contains characters other than 'a' and 'h'?]" ~: seeDoctor "" "x" ~=? False
    , "[seeDoctor: Do you consider the case that the second input string contains characters other than 'a' and 'h'?]" ~: seeDoctor "aah" "aaaxh" ~=? False
    , "[seeDoctor: Do you consider the case that the second input string contains characters other than 'a' and 'h'?]" ~: seeDoctor "aah" "aaahx" ~=? False
    , "[seeDoctor: Do you consider the case that both input strings contain characters other than 'a' and 'h'?]" ~: seeDoctor "x" "x" ~=? False
    , "[seeDoctor: Do you consider the case that the first input string contains an 'h' but does not end with 'h'?]" ~: seeDoctor "aaaha" "aaaah" ~=? False
    , "[seeDoctor: Do you consider the case that the second input string contains an 'h' but does not end with 'h'?]" ~: seeDoctor "aaah" "aaaaha" ~=? False
    , "[seeDoctor: Do you consider the case that both input strings contain an 'h' but do not end with 'h'?]" ~: seeDoctor "aaaha" "aaaaha" ~=? False
    , "[seeDoctor: Do you consider the case that the first input string ends with two 'h's?]" ~: seeDoctor "aaahh" "aaaah" ~=? False
    , "[seeDoctor: Do you consider the case that the second input string ends with two 'h's?]" ~: seeDoctor "aaah" "aaaahh" ~=? False
    , "[seeDoctor: Do you consider the case that both input strings end with two 'h's?]" ~: seeDoctor "aaahh" "aaaahh" ~=? False
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

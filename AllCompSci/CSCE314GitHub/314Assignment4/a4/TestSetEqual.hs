module TestSetEqual where

import Test.HUnit
import System.Exit
import Main

myTestList = 
  TestList [ 
      "An elementary case failed! (True case, taking two strings as input)"  ~: setEqual ("haskel") ("leksah") ~=? True
    , "An elementary case failed! (True case, taking two empty strings as input)"  ~: setEqual "" "" ~=? True
    , "An elementary case failed! (True case, taking two integer lists as input)"  ~: setEqual [1,2,3,4] [2,1,3,4] ~=? True
    , "An elementary case failed! (True case, taking two empty integer lists as input)"  ~: setEqual ([]::[Int]) ([]::[Int]) ~=? True
    , "An elementary case failed! (True case, taking two string lists as input)"  ~: setEqual ["haskell","java","c++","python"] ["haskell","java","c++","python"] ~=? True
    , "An elementary case failed! (False case, taking two strings as input)"  ~: setEqual "haskel" "eksah" ~=? False
    , "An elementary case failed! (False case, taking an empty string as the second input)"  ~: setEqual "haskel" "" ~=? False
    , "An elementary case failed! (False case, taking two integer lists as input)"  ~: setEqual [1,2,3,4] [1,3,4] ~=? False
    , "An elementary case failed! (False case, taking an empty integer list as the second input)"  ~: setEqual [1,2,3,4] ([]::[Int]) ~=? False
    , "An elementary case failed! (False case, taking two string lists as input)"  ~: setEqual ["haskell","java","c++","python"] ["haskell","java","c++"] ~=? False
    , "An elementary case failed! (False case, taking an empty string list as the first input)"  ~: setEqual [] ["haskell","java","c++"] ~=? False
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

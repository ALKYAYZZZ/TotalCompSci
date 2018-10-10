module TestAnagramEncode where

import Test.HUnit
import System.Exit
import Main

myTestList = 
  TestList [ 
    "An elementary case failed!"  ~: anagramEncode "Chig-gar-roo-gar-rem/Chig-gar-roo-gar-rem/Rough Tough! Real Stuff!" ~=? "Cihhg i-Tggo-augrga-hrr!-o roRo-eoga-algr a-Srrt-eurmfe/fmR!/oXCuXhgX"
    , "An elementary case failed!"  ~: anagramEncode "Hello, Welcome to Haskell World!" ~=? "Hoeemllell  otW,oo  rWHleadls!ckX"
    , "An elementary case failed!"  ~: anagramEncode "Good Bye, Haskell, I will go to Java!" ~=? "GIo owdi lBly eg,o  Htaos kJealvla,! X"
    , "An empty string case failed!" ~: anagramEncode "" ~=? "XXXXXX"
    , "An special case XXXXXXXa failed!" ~: anagramEncode "XXXXXXa" ~=? "XXXXXaXX"
    , "An special case XXXXXXXaXXXXa failed!" ~: anagramEncode "XXXXXXXaXXXXa" ~=? "XaXXXXXXXXXaXX"
    , "An special case XXx, which ends with lower case x failed!" ~: anagramEncode "XXx" ~=? "XXXXxX"
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

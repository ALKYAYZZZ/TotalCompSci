module TestAnagramDecode where

import Test.HUnit
import System.Exit
import Main

myTestList = 
  TestList [ 
    "An elementary case failed!"  ~: anagramDecode "Cihhg i-Tggo-augrga-hrr!-o roRo-eoga-algr a-Srrt-eurmfe/fmR!/oXCuXhgX" ~=? "Chig-gar-roo-gar-rem/Chig-gar-roo-gar-rem/Rough Tough! Real Stuff!"
    , "An elementary case failed!"  ~: anagramDecode "Hoeemllell  otW,oo  rWHleadls!ckX" ~=? "Hello, Welcome to Haskell World!"
    , "An elementary case failed!"  ~: anagramDecode "GIo owdi lBly eg,o  Htaos kJealvla,! X" ~=? "Good Bye, Haskell, I will go to Java!"
    , "An empty string case failed!" ~: anagramDecode "XXXXXX" ~=? ""
    , "An special case, where the original message is XXXXXXXa, failed!" ~: anagramDecode "XXXXXaXX" ~=? "XXXXXXa" 
    , "An special case, where the original message is XXXXXXXaXXXXa, failed!" ~: anagramDecode "XaXXXXXXXXXaXX" ~=? "XXXXXXXaXXXXa"
    , "An special case, where the original message is XXx that ends with lower case x, failed!" ~: anagramDecode "XXXXxX" ~=? "XXx" 
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

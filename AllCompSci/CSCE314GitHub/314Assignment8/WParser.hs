module WParser ( parse,
                 wprogram ) where

    import Data.Char
    import W

    import Control.Applicative (Applicative(..))
    import Control.Monad (liftM, ap)

    -----------------------------
    -- This is the main parser --
    -----------------------------
    wprogram = whitespace >> many stmt >>= \ss -> return (Block ss)
    -- a program is a sequence of statements; the parser returns them
    -- as a single block-statement

    -- only two of the statement types above are supported, the rest are undefined.
    -- please implement them
	
	--Each of the statements are made using the stmt command within them in order to recursively solve any
	--of the problems the parser may have, this is seen more so on the block statement parser, that accepts multiple
	--statements at once.
    stmt = varDeclStmt +++ assignStmt +++ ifStmt +++ whileStmt +++ 
           blockStmt +++ emptyStmt +++ printStmt

    emptyStmt = 
      symbol ";" >>
      return Empty
  
    printStmt = 
      keyword "print" >>
      expr >>= \e ->
      symbol ";" >>
      return (Print e)

    varDeclStmt = 
        keyword "var" >>
        identifier >>= \t ->
        symbol "=" >> 
        expr >>= \e ->
        symbol ";" >>
        return (VarDecl t e)
		
	
    assignStmt = 
        identifier >>= \t ->
        symbol "=" >>
        expr >>= \e ->
        symbol ";" >>
        return (Assign t e)
		
    ifStmt = 
		keyword "if" >>
		parens expr >>= \cond ->
		stmt >>= \s1 ->
		keyword "else" >>
		stmt >>= \s2 ->
		return (If cond s1 s2)
		
    whileStmt = 
        keyword "while" >>
        parens expr >>= \cond ->
        stmt >>= \s ->
        return (While cond s)
		
    blockStmt = 
        symbol "{" >>
        many stmt >>= \stmts ->
        symbol "}" >>
        return (Block stmts)

    -- the only kind of expression supported for now is stringLiterals
    -- The full language of w is implemented by trickling down the parsing library, all logic operators, numeric operators
	--and variable operators are accounted for using the parsing syntax.
    expr' = stringLiteral
	
    expr = logic >>= logicSeq
	
    logicSeq l = (
				(symbol "&&" >> return And) +++ (symbol "||" >> return Or) >>= \op ->
				logic >>= \r ->
				logicSeq (op l r)
				 )
				 +++ return l
    logic = (
			symbol "!" >>
			logic >>= \x ->
			return (Not x)
			)
			+++ comparison
			
    comparison = arithmatic >>= comparisonSeq
	
    comparisonSeq l = (
					  (symbol "==" >> return Equals) +++ (symbol "!=" >> return NotEqual) +++ (symbol ">=" >> return GreaterOrEqual) +++ (symbol ">" >> return Greater) +++ (symbol "<=" >> return LessOrEqual) +++ (symbol "<" >> return Less) >>= \op ->
					  arithmatic >>= \r ->
					  comparisonSeq (op l r)
					  )
					  +++ return l
    arithmatic = term >>= termSeq
	
    termSeq l = (
				(symbol "+" >> return Plus) +++ (symbol "-" >> return Minus) >>= \op ->
				term >>= \r ->
				termSeq (op l r)
				)
				+++ return l
    term = factor >>= factorSeq
	
    factorSeq l = (
				  (symbol "*" >> return Multiplies) +++ (symbol "/" >> return Divides) >>= \op ->
				  factor >>= \r ->
				  factorSeq (op l r)
				  )
				  +++ return l
    factor = (nat >>= \n -> return $ (Val (VInt n))) +++ variable +++ stringLiteral +++ parens expr +++ bool			  
	
	-- Boolean Literals
    bool = (keyword "true" >> return (Val(VBool True))) +++
		   (keyword "false" >> return (Val (VBool False)))
    -- stringLiterals can contain \n characters
    stringLiteral = char ('"') >>
                    many stringChar >>= \s ->
                    char ('"') >>
                    whitespace >>
                    return (Val (VString s))

    stringChar = (char '\\' >> char 'n' >> return '\n') 
                 +++ sat (/= '"')

    ----------------------
    -- Parser utilities --
    ----------------------

    keywords = words "var if else while"
    isKeyword s = s `elem` keywords

    keyword s = 
      identifier >>= \s' ->
      if s' == s then return s else failure 
	 
	--Used to check to see if the text is a variable or a keyword based on if it
	--has been set in memory yet.
    variable = 
      identifier >>= \s ->
      if isKeyword s then failure else return (Var s)	  
       
    newtype Parser a = P (String -> [(a, String)])
    
    parse :: Parser a -> String -> [(a, String)]
    parse (P p) inp = p inp
    
    instance Functor Parser where
        fmap = liftM
     
    instance Applicative Parser where
        pure  = return
        (<*>) = ap
    
    instance Monad Parser where
        -- return :: a -> Parser a
        return v = P $ \inp -> [(v, inp)]
                 
        -- (>>=) :: Parser a -> (a -> Parser b) -> Parser b
        p >>= q = P $ \inp -> case parse p inp of 
                                [] -> []
                                [(v, inp')] -> let q' = q v in parse q' inp'
    
    failure :: Parser a
    failure = P $ \_ -> []
    
    item :: Parser Char 
    item = P $ \inp -> case inp of 
                         (x:xs) -> [(x, xs)]
                         [] -> []
    
    -- Parse with p or q
    (+++) :: Parser a -> Parser a -> Parser a
    p +++ q = P $ \inp -> case parse p inp of 
                              [] -> parse q inp
                              [(v, inp')] -> [(v, inp')]
    
    
    -- Simple helper parsers
    sat :: (Char -> Bool) -> Parser Char
    sat pred = item >>= \c ->
               if pred c then return c else failure
    
    digit, letter, alphanum :: Parser Char
    digit = sat isDigit
    letter = sat isAlpha
    alphanum = sat isAlphaNum
    
    char :: Char -> Parser Char
    char x = sat (== x)
    
    string = sequence . map char 
    
    many1 :: Parser a -> Parser [a]
    many1 p = p >>= \v ->
              many p >>= \vs ->
              return (v:vs)
    
    many :: Parser a -> Parser [a]
    many p = many1 p +++ return []
    
    -- Useful building blocks
    nat :: Parser Int
    nat = many1 digit >>= \s ->
          whitespace >>
          return (read s)
    
    identifier :: Parser String
    identifier = letter >>= \s ->
                 many alphanum >>= \ss ->
                 whitespace >>
                 return (s:ss)
    
    whitespace :: Parser ()
    whitespace = many (sat isSpace) >> comment
    
    symbol s = 
        string s >>= \s' ->
        whitespace >>
        return s'    
    
    comment = ( string "//" >>
                many (sat (/= '\n')) >>
                whitespace ) +++ return ()
    
    parens p = 
        symbol "(" >> 
        p >>= \res ->
        symbol ")" >>
        return res

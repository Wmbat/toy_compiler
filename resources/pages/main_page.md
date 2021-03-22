@mainpage

# What is toy_compiler?

**toy_compiler** is a university project for the **Compiler Design** class. Whose purpose is to implement a fully functional compiler for a language defined by
the professor.

# Lexing

The lexing is performed using the following Regex rules:

```
identifiers    ::= ^[a-zA-Z]([a-zA-Z0-9_])*
integer        ::= (^[1-9][0-9]*)|^0$
float          ::= (integer)[.](([0-9]*[1-9]$)|0$)(e(+|-)?integer)?
string         ::= "(([a-zA-Z0-9_])| )*"
```

# Grammar

The grammar of the language compiled by the compiler is presented below in it's **LL(1)** form.

```
<START>        ::= <prog>
<prog>         ::= {{<classDecl>}} {{<funcDef>}} 'main' <funcBody>
<classDecl>    ::= 'class' 'id' [['inherits' 'id' {{',' 'id'}}]] '{' {{<visibility> <memberDecl>}} '}' ';'
<visibility>   ::= 'public' | 'private' | EPSILON
<memberDecl>   ::= <funcDecl> | <varDecl>  
<funcDecl>     ::= 'func' 'id' '(' <fParams> ')' ':' <type> ';' 
                |  'func' 'id' '(' <fParams> ')' ':' 'void' ';' 
<funcHead>     ::= 'func' [['id' 'sr']] 'id' '(' <fParams> ')' ':' <type> 
                |  'func' [['id' 'sr']] 'id' '(' <fParams> ')' ':' 'void'
<funcDef>      ::= <funcHead> <funcBody> 
<funcBody>     ::= '{' [[ 'var' '{' {{<varDecl>}} '}' ]] {{<statement>}} '}'
<varDecl>      ::= <type> 'id' {{<arraySize>}} ';'
<statement>    ::= <assignStat> ';'
                |  'if'     '(' <relExpr> ')' 'then' <statBlock> 'else' <statBlock> ';'
                |  'while'  '(' <relExpr> ')' <statBlock> ';'
                |  'read'   '(' <variable> ')' ';'
                |  'write'  '(' <expr> ')' ';'
                |  'return' '(' <expr> ')' ';'
                |  'break' ';'
                |  'continue' '; '
                |  <functionCall> ';'
<assignStat>   ::= <variable> <assignOp> <expr>
<statBlock>    ::= '{' {{<statement>}} '}' | <statement> | EPSILON  
<expr>         ::= <arithExpr> | <relExpr>
<relExpr>      ::= <arithExpr> <relOp> <arithExpr>
<arithExpr>    ::= <arithExpr> <addOp> <term> | <term> 
<sign>         ::= '+' | '-'
<term>         ::= <term> <multOp> <factor> | <factor>
<factor>       ::= <variable>
                |  <functionCall>
                |  'intLit' | 'floatLit' | 'stringLit' 
                |  '(' <arithExpr> ')'
                |  'not' <factor>
                |  <sign> <factor>
                |  'qm' '[' <expr> ':' <expr> ':' <expr> ']' 
<variable>     ::= {{<idnest>}} 'id' {{<indice>}}
<functionCall> ::= {{<idnest>}} 'id' '(' <aParams> ')'
<idnest>       ::= 'id' {{<indice>}} '.'
                |  'id' '(' <aParams> ')' '.'
<indice>       ::= '[' <arithExpr> ']'
<arraySize>    ::= '[' 'intNum' ']' | '[' ']'
<type>         ::= 'integer' | 'float' | 'string' | 'id'
<fParams>      ::= <type> 'id' {{<arraySize>}} {{<fParamsTail>}} | EPSILON  
<aParams>      ::= <expr> {{<aParamsTail>}} | EPSILON 
<fParamsTail>  ::= ',' <type> 'id' {{<arraySize>}}
<aParamsTail>  ::= ',' <expr>
<assignOp>     ::= '='
<relOp>        ::= 'eq' | 'neq' | 'lt' | 'gt' | 'leq' | 'geq' 
<addOp>        ::= '+' | '-' | 'or' 
<multOp>       ::= '*' | '/' | 'and'
```

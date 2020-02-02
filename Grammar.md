Boa's grammar is similar in design to that of popular programming languages suchs as
Python and Ruby. The program is split into a block that contains variable
declarations, function definitions, class definitions and expressions that alter data
`a = 1 + 2`.

This grammar will be added to over time and is not a rigid definition of future
language syntax.


```
block   ::=  ( var_dec | func_def | class_def | expression ) +

var_dec ::= "var" ident = expression ';'

atom ::= ( ident | literal )

expression ::=  atom | unary_op atom | atom binary_op atom

unary_op ::= '-' | '++' | '--'

binary_op ::= '+' | '-' | '/' | '*'

ident ::= /^[\w][\w\d]*/

func_def ::= 'def' ident( argument* ) 'begin' block+ 'end'

argument ::= ident | ident '=' literal

class_def ::= 'class' ident 'begin' block+ 'end'
```
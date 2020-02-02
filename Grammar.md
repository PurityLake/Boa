```
block   ::=  ( var_dec | func_dec | class_dec | expression ) +

var_dec ::= "var" ident = expression ';'

atom ::= ( ident | literal )

expression ::=  atom | unary_op atom | atom binary_op atom

unary_op ::= '-' | '++' | '--'

binary_op ::= '+' | '-' | '/' | '*'

ident ::= /^[\w][\w\d]*/

func_dec ::= 'def' ident( argument* ) 'begin' block+ 'end'

class_dec ::= 'class' ident 'begin' block+ 'end'
```
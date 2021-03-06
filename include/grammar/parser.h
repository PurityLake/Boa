#ifndef __H_PARSER__
#define __H_PARSER__

#include "grammar/lexer.h"
#include "ast/ast.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

Node *block();

void var_dec(Node *node);
void func_def(Node *node);

void ident(Node *node);
void number(Node *node);
int op(Node *node);
void expression(Node *node);
void param_list(Node *node);
void set_parser_tokens(TokenList *list);
Node *parse_list();

#ifdef __cplusplus
}
#endif

#endif /* __H_PARSER__ */

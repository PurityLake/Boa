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
int op(Node *node, int give_error);
void expression(Node *node);
void param_list(Node *node);
Node *parse_list(TokenList *list);

#ifdef __cplusplus
}
#endif

#endif /* __H_PARSER__ */

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

static int _is_error = 0;
static Token **_curr;

Node *block();

void ident(Node *node);
void number(Node *node);
int op(Node *node, int give_error);
void expression(Node *node);
void var_dec(Node *node);
void param_list(Node *node);
void func_dec(Node *node);
Node *parse_line(Token**);

#ifdef __cplusplus
}
#endif

#endif /* __H_PARSER__ */

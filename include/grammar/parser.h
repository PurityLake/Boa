#ifndef __H_PARSER__
#define __H_PARSER__

#include "lexer.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

static int _is_error = 0;
static token **_curr;

void block();

void ident();
void number();
int op(int give_error);
void expression();
void var_dec();
void param_list();
void func_dec();
void parse_line(token **line);

#ifdef __cplusplus
}
#endif

#endif /* __H_PARSER__ */

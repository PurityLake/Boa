#ifndef __H_AST__
#define __H_AST__

#include "grammar/lexer.h"
#include "grammar/parser.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _node {
    char *value;
} Node;

#ifdef __cplusplus
}
#endif

#endif /* __H_AST__ */

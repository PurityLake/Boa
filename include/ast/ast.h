#ifndef __H_AST__
#define __H_AST__

#include "grammar/lexer.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _node {
    Token *token;
    struct _node *parent;
    struct _node *left, *right;
} Node;

Node *create_Node(Token *token);
Node *create_with_parent_Node(Token *token, Node *parent);
void free_Node(Node *node);
void print_Node(Node *node);

#ifdef __cplusplus
}
#endif

#endif /* __H_AST__ */

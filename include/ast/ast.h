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

Node *create_node(Token *token);
Node *create_node_with_parent(Token *token, Node *parent);
void free_node(Node *node);
void print_node(Node *node);

#ifdef __cplusplus
}
#endif

#endif /* __H_AST__ */

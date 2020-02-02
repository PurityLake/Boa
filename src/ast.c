#include "ast/ast.h"

#ifdef __cplusplus
extern "C" {
#endif

Node *create_node(Token *token) {
    Node *out = (Node *)malloc(sizeof(Node));
    out->token = token;
    out->parent = NULL;
    out->left = NULL;
    out->right = NULL;
    return out;
}

Node *create_node_with_parent(Token *token, Node *parent) {
    Node *out = create_node(token);
    out->parent = parent;
    return out;
}

void print_node(Node *node) {
    if (node == NULL) return;
    print_node(node->left);
    if (node->token != NULL) {
        printf("%s ", node->token->value);
        fflush(stdout);
    }
    print_node(node->right);
    fflush(stdout);
}

#ifdef __cplusplus
}
#endif

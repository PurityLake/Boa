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

void free_node(Node *node) {
    if (node == NULL) return;
    free_node(node->left);
    free(node);
    free_node(node->right);
}

void print_nodel(Node *node) {
    if (node == NULL) return;
    print_nodel(node->left);
    if (node->token != NULL) {
        printf("%s ", node->token->value);
        fflush(stdout);
    }
    print_nodel(node->right);
    fflush(stdout);
}

void print_noder(Node *node) {
    if (node == NULL) return;
    print_noder(node->right);
    if (node->token != NULL) {
        printf("%s ", node->token->value);
        fflush(stdout);
    }
    print_noder(node->left);
    fflush(stdout);
}

void print_node(Node *node) {
    if (node->left != NULL && node->left->token->type == T_FUNC_DEF) {
        print_noder(node);
    } else {
        print_nodel(node);
    }
}

#ifdef __cplusplus
}
#endif

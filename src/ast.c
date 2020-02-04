#include "ast/ast.h"

#ifdef __cplusplus
extern "C" {
#endif

Node *create_Node(Token *token) {
    Node *out = (Node *)malloc(sizeof(Node));
    out->token = token;
    out->parent = NULL;
    out->left = NULL;
    out->right = NULL;
    return out;
}

Node *create_with_parent_Node(Token *token, Node *parent) {
    Node *out = create_Node(token);
    out->parent = parent;
    return out;
}

void free_Node(Node *node) {
    if (node == NULL) return;
    free_Node(node->left);
    if (node->token != NULL && node->token->type > T_PARSER_START) {
        free_nostr_Token(node->token);
    }
    free_Node(node->right);
    free(node);
}

void print_nodel(Node *node) {
    if (node == NULL) return;
    print_nodel(node->left);
    if (node->token != NULL) {
        if (node->token->type > T_PARSER_START) {
            if (node->token->type == T_SPLIT) {
                printf("\n");
            }
        } else {
            printf("%s ", node->token->value);
            fflush(stdout);
        }
    }
    print_nodel(node->right);
    fflush(stdout);
}

void print_noder(Node *node) {
    if (node == NULL) return;
    print_noder(node->right);
    if (node->token != NULL) {
        if (node->token->type > T_PARSER_START) {
            if (node->token->type == T_VAR_DEC) {
                print_nodel(node->left);
                return;
            } else if (node->token->type == T_SPLIT) {
                printf("\n");
            }
        } else {
            printf("%s ", node->token->value);
            fflush(stdout);
        }
    }
    print_noder(node->left);
    fflush(stdout);
}

void print_Node(Node *node) {
    if (node->left != NULL && node->left->token != NULL && node->left->token->type == T_FUNC_DEF) {
        print_noder(node);
    } else {
        print_nodel(node);
    }
}

#ifdef __cplusplus
}
#endif

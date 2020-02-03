#include "grammar/parser.h"

#ifdef __cplusplus
extern "C" {
#endif

static int _is_error = 0;
static TokenList *_curr;

void next_tok() {
    _curr = _curr->next;
}

void error(char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    _is_error = 1;
}

int accept(char *val) {
    if (_is_error) return 0;
    if (_curr->token == NULL) return 0;
    if (strcmp(_curr->token->value, val) == 0) {
        next_tok();
        return 1;
    }
    return 0;
}

int expect(char *val, char *expected_fmt) {
    if (_is_error) return 0;
    if (accept(val)) {
        return 1;
    }
    _curr = _curr->prev;
    error(expected_fmt, _curr->token->line, _curr->token->col);
    return 0;
}

int accept_type(int type) {
    if (_is_error) return 0;
    if (_curr->token == NULL) return 0;
    if (_curr->token->type == type) {
        next_tok();
        return 1;
    }
    return 0;
}

void ident(Node *node) {
    if (_is_error) return;
    node->token = _curr->token;
    if (!accept_type(T_IDENT)) {
        error("Invalud identifier: '%s'", _curr->token->value);
        return;
    }
}

void number(Node *node) {
    if (_is_error) return;
    size_t len = strlen(_curr->token->value);
    for (int i = 0; i < len; i++) {
        if (!isdigit(_curr->token->value[i])) {
            error("Invalid number: '%s'\n", _curr->token->value);
            return;
        }
    }
    accept(_curr->token->value);
}

int op(Node *node, int give_error) {
    if (_is_error) return 0;
    if (accept("+") || accept("-") || accept("*") || accept("/") || accept("=")) {
        if (give_error) error("Invalid op: '%s'\n", _curr->token->value);
        node->token = create_token(_curr->prev->token->type, _curr->prev->token->value, 
                            _curr->prev->token->line, _curr->prev->token->col);
        return 1;
    }
    node->token = NULL;
    return 0;
}

void expression(Node *node) {
    if (_is_error) return;
    Node *i = create_node_with_parent(NULL, node);
    Node *o = create_node_with_parent(NULL, node);
    ident(i);
    while (op(o, 0)) {
        node->token = o->token;
        node->left = i;
        node->right = create_node_with_parent(NULL, node);
        node = node->right;
        i = create_node_with_parent(NULL, node);
        o = create_node_with_parent(NULL, node);
        ident(i);
    }
    node->left = i;
}

void var_dec(Node *node) {
    if (_is_error) return;
    node->left->token = create_token(T_VAR_DEC, "VAR_DEC", _curr->token->line, _curr->token->col);
    node = node->left;
    Node *i = create_node_with_parent(NULL, node);
    ident(i);
    if (expect("=", "Line %d:%d: Expected a '=' in variable declation!")) {
        node->left = create_node_with_parent(create_token(T_EQ, "=", _curr->prev->token->line, _curr->prev->token->col), node);
        node = node->left;
        node->left = create_node_with_parent(i->token, node);
        node->right = create_node_with_parent(NULL, node);
        expression(node->right);
    }
}

void func_def(Node *node) {
    node->left->token = create_token(T_FUNC_DEF, "FUNC_DEF", _curr->token->line, _curr->token->col);
    node = node->left;
    Node *i = create_node_with_parent(NULL, node);
    Node *p = create_node_with_parent(NULL, i);
    ident(i);
    param_list(p);
    node->left = i;
    node->left->left = p;
}

void param_list(Node *node) {
    if (_is_error) return;
    expect("(", "Line %d:%d: Excected ')' at the start of a parameter list.");
    node->token = create_token(T_PARAM_LIST, "PARAM_LIST", _curr->token->line, _curr->token->col);
    Node *i = create_node_with_parent(NULL, node);
    while (_curr->token->type != T_RPAREN) {
        ident(i);
        if (accept("=")) {
            Node *split = create_node_with_parent(create_token(T_SPLIT, "", -1, -1), node);
            Node *op = create_node_with_parent(_curr->prev->token, split);
            Node *j = create_node_with_parent(NULL, op);
            i->parent = op;
            ident(j);
            split->right = op;
            op->left = j;
            op->right = i;
            node->left = split;
            node = node->left;
            i = create_node_with_parent(NULL, node);
        } else {
            node->left = i;
            node = node->left;
            i = create_node_with_parent(NULL, node);
        }
        if (accept(",")) continue;
    }
    expect(")", "Line %d:%d: Excected ')' at the end of a parameter list.");
}

Node *block() {
    if (_is_error) return NULL;
    if (_curr == NULL) return NULL;
    Node *node = create_node(create_token(T_BLOCK, "BLOCK", _curr->token->line, _curr->token->col));
    node->left = create_node_with_parent(NULL, node);
    if (accept("var")) {
        var_dec(node);
        expect(";", "Line %d:%d: Excected ';' at the end of a statement!");
    } else if (accept("def")) {
        func_def(node);
    } else {
        return NULL;
    }
    return node;
}

void set_parser_tokens(TokenList *list) {
    _curr = list;
}

Node *parse_list() {
    return block();
}

#ifdef __cplusplus
}
#endif

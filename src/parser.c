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
    if (_curr == NULL || _curr->token == NULL) return 0;
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
    if (!accept_type(T_IDENT)) error("Invalud identifier: '%s'", _curr->token->value);
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

int op(Node *node) {
    if (_is_error) return 0;
    if (accept("+") || accept("-") || accept("*") || accept("/") || accept("=")) {
        node->token = _curr->prev->token;
        return 1;
    }
    node->token = NULL;
    return 0;
}

void expression(Node *node) {
    if (_is_error) return;
    Node *i = create_with_parent_Node(NULL, node);
    Node *o = create_with_parent_Node(NULL, node);
    ident(i);
    while (op(o)) {
        node->token = o->token;
        free(o);
        node->left = i;
        node->right = create_with_parent_Node(NULL, node);
        node = node->right;
        i = create_with_parent_Node(NULL, node);
        o = create_with_parent_Node(NULL, node);
        ident(i);
    }
    if (o != NULL) free(o);
    node->left = i;
}

void var_dec(Node *node) {
    if (_is_error) return;
    node->left = create_with_parent_Node(NULL, node);
    node->left->token = create_Token(T_VAR_DEC, "VAR_DEC", _curr->token->line, _curr->token->col);
    node = node->left;
    Node *i = create_with_parent_Node(NULL, node);
    ident(i);
    if (expect("=", "Line %d:%d: Expected a '=' in variable declation!")) {
        node->left = create_with_parent_Node(_curr->prev->token, node);
        node = node->left;
        i->parent = node;
        node->left = i;
        node->right = create_with_parent_Node(NULL, node);
        expression(node->right);
    }
}

void func_def(Node *node) {
    node->left = create_with_parent_Node(create_Token(T_FUNC_DEF, "FUNC_DEF", _curr->token->line, _curr->token->col), node);
    node = node->left;
    Node *i = create_with_parent_Node(NULL, node);
    Node *p = create_with_parent_Node(NULL, i);
    ident(i);
    param_list(p);
    node->left = i;
    node->left->left = p;
    expect("begin", "Line %d:%d: Expected a 'begin' at beginning of function definition!\n");
    node->right = create_with_parent_Node(create_Token(T_SPLIT, "", -1, -1), node);
    node = node->right;
    node->right = block();
    while(node->right != NULL) {
        node->right->parent = node;
        node->left = create_with_parent_Node(create_Token(T_SPLIT, "", -1, -1), node);
        node = node->left;
        node->right = block();
    }
    expect("end", "Line %d:%d: Expected a 'end' at end of function definition!\n");
}

void param_list(Node *node) {
    if (_is_error) return;
    expect("(", "Line %d:%d: Excected ')' at the start of a parameter list.\n");
    node->token = create_Token(T_PARAM_LIST, "PARAM_LIST", _curr->token->line, _curr->token->col);
    Node *i = create_with_parent_Node(NULL, node);
    while (_curr->token->type != T_RPAREN) {
        ident(i);
        if (accept("=")) {
            Node *split = create_with_parent_Node(create_Token(T_SPLIT, "", -1, -1), node);
            Node *op = create_with_parent_Node(_curr->prev->token, split);
            Node *j = create_with_parent_Node(NULL, op);
            i->parent = op;
            ident(j);
            split->right = op;
            op->left = j;
            op->right = i;
            node->left = split;
            node = node->left;
            i = create_with_parent_Node(NULL, node);
        } else {
            node->left = i;
            node = node->left;
            i = create_with_parent_Node(NULL, node);
        }
        accept(",");
    }
    if (i != NULL) free(i);
    expect(")", "Line %d:%d: Excected ')' at the end of a parameter list.");
}

Node *block() {
    if (_is_error) return NULL;
    if (_curr == NULL || _curr->next == NULL) return NULL;
    Node *node = create_Node(create_Token(T_BLOCK, "BLOCK", _curr->token->line, _curr->token->col));
    if (accept("var")) {
        var_dec(node);
        expect(";", "Line %d:%d: Excected ';' at the end of a statement!");
    } else if (accept("def")) {
        func_def(node);
    } else {
        free_Node(node);
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

#include "parser.h"

#ifdef __cplusplus
extern "C" {
#endif

void next_tok() {
    ++_curr;
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
    if ((*_curr) == NULL) return 0;
    if (strcmp((*_curr)->value, val) == 0) {
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
    --_curr;;
    error(expected_fmt, (*_curr)->line, (*_curr)->col);
    return 0;
}

int accept_type(int type) {
    if (_is_error) return 0;
    if ((*_curr) == NULL) return 0;
    if ((*_curr)->type == type) {
        next_tok();
        return 1;
    }
    return 0;
}

void ident() {
    if (_is_error) return;
    printf("ident: %s\n", (*_curr)->value);
    if (!accept_type(T_IDENT)) {
        error("Invalud identifier: '%s'", (*_curr)->value);
        return;
    }
}

void number() {
    if (_is_error) return;
    size_t len = strlen((*_curr)->value);
    for (int i = 0; i < len; i++) {
        if (!isdigit((*_curr)->value[i])) {
            error("Invalid number: '%s'\n", (*_curr)->value);
            return;
        }
    }
    printf("number: %s\n", (*_curr)->value);
    accept((*_curr)->value);
}

int op(int give_error) {
    if (_is_error) return 0;
    if (accept("+") || accept("-") || accept("*") || accept("/") || accept("=")) {
        printf("op: %s\n", (*(_curr - 1))->value);
        if (give_error) error("Invalid op: '%s'\n", (*_curr)->value);
        return 1;
    }
    return 0;
}

void expression() {
    if (_is_error) return;
    printf("expression\n");
    ident();
    while (op(0)) {
        ident();
    }
}

void var_dec() {
    if (_is_error) return;
    printf("var_dec\n");
    ident();
    if (expect("=", "Line %d:%d: Expected a '=' in variable declation!")) {
        expression();
    }
}

void param_list() {
    if (_is_error) return;
    printf("param_list\n");
    expect("(", "Line %d:%d: Excected ')' at the start of a parameter list.");
    while ((*_curr)->type != T_RPAREN) {
        ident();
        if (accept(",")) continue;
        if (accept("=")) {
            printf("param_list: =\n");
            ident();
        }
    }
    expect(")", "Line %d:%d: Excected ')' at the end of a parameter list.");
}

void func_dec() {
    if (_is_error) return;
    printf("func_dec\n");
    ident();
    param_list();
}

void block() {
    if (_is_error) return;
    printf("block\n");
    if (accept("var")) {
        var_dec();
        expect(";", "Line %d:%d: Excected ';' at the end of a statement!");
    } else if (accept("def")) {
        func_dec();
    }
}

void parse_line(token **line) {
    _curr = line;
    block();
}

#ifdef __cplusplus
}
#endif

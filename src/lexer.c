#include "grammar/lexer.h"

#ifdef __cplusplus
extern "C" {
#endif

Token *create_token(int type, char *value, unsigned int line, unsigned int col) {
    Token *ret = (Token *)malloc(sizeof(Token));
    ret->type = type;
    ret->line = line;
    ret->col = col;
    ret->value = value;
    return ret;
}

void free_token(Token **Token) {
    free((*Token)->value);
    (*Token)->value = NULL;
    free(*Token);
}

Token **create_token_array(unsigned int length) {
    Token **ret = (Token**)malloc(sizeof(Token*) * length);
    for (int i = 0; i < length; ++i) {
        ret[i] = NULL;
    }
    return ret;
}

void free_token_array(Token ***arr) {
    Token **a = *arr;
    while ((*a) != NULL) {
        free_token(a);
        ++a;
    }
}

Token *lexeme_to_token(const char *lexeme, int len, int lineno, int col) {
    char *resized_lexeme = (char *)malloc(sizeof(char) * len);
    memset(resized_lexeme, '\0', len);
    strncpy(resized_lexeme, lexeme, len);

    for (int i = 0; i < TABLE_SIZE; ++i) {
        _translation_table_entry entry = _trans_table[i];
        if (strcmp(resized_lexeme, entry.name) == 0) {
            return create_token(entry.type, resized_lexeme, lineno, col);
        }
    }
    return create_token(T_IDENT, resized_lexeme, lineno, col);
}

Token **lex_line(const char *line_text, unsigned int lineno) {
    Token **arr = create_token_array(MAX_TOKENS_PER_LINE);
    char buf[MAX_LEXEME_SIZE];
    int buf_idx = 0;
    int start_tok = 0;
    int line_idx = 0;
    int i = 0;
    memset(buf, '\0', MAX_LEXEME_SIZE);
    while (line_text[line_idx] != '\n' && line_text[line_idx] != '\0') {
        if (isspace(line_text[line_idx])) {
            if (buf_idx > 0) {
                arr[i++] = lexeme_to_token(buf, buf_idx+1, lineno, (start_tok == -1) ? line_idx : start_tok);
                memset(buf, '\0', buf_idx);
                buf_idx = 0;
                start_tok = -1;
            }
        } else {
            if (start_tok == -1) start_tok = line_idx;
            if (is_op(line_text[line_idx]) == 0) {
                if (buf_idx > 0) {
                    arr[i++] = lexeme_to_token(buf, buf_idx+1, lineno, start_tok);
                    memset(buf, '\0', buf_idx);
                    buf_idx = 0;
                }
                buf[buf_idx++] = line_text[line_idx];
                arr[i++] = lexeme_to_token(buf, buf_idx+1, lineno, line_idx);
                memset(buf, '\0', buf_idx);
                buf_idx = 0;
                start_tok = -1;
            } else {
                buf[buf_idx++] = line_text[line_idx];
            }
        }
        ++line_idx;
    }
    if (buf_idx > 0) {
        arr[i] = lexeme_to_token(buf, buf_idx+1, lineno, start_tok);
    }
    return arr;
}

int is_op(char c) {
    if (c == '*' || c == '/' || c == '+' || c == '-' || c == '=' || c == ';' ||
        c == '(' || c == ')' || c == '{' || c == '}' || c == ',' || c == '.') return 0;
    return -1;
}

#ifdef __cplusplus
}
#endif

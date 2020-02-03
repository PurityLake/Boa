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

void free_token(Token *token) {
    free(token->value);
    token->value = NULL;
    free(token);
    token = NULL;
}

TokenList *create_token_list() {
    TokenList *out = (TokenList *)malloc(sizeof(TokenList));
    out->token = NULL;
    out->prev = NULL;
    out->next = NULL;
    return out;
}

void add_to_token_list(TokenList *list, Token *token) {
    if (list == NULL) return;
    if (list->token == NULL) {
        list->token = token;
        list->next = create_token_list();
        list->next->prev = list;
    } else {
        TokenList *temp = list;
        while (temp->token != NULL) {
            temp = temp->next;
        }
        temp->token = token;
        temp->next = create_token_list();
        temp->next->prev = temp;
    }
}

void free_token_list(TokenList *list) {
    if (list->token != NULL) {
        free_token_list(list->next);
        free_token(list->token);
    }
    free(list);
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

TokenList *lex_file(FILE *f) {
    TokenList *list = create_token_list();
    char buf[MAX_LEXEME_SIZE];
    int lineno = 1;
    int buf_idx = 0;
    int start_tok = 0;
    int line_idx = 0;
    memset(buf, '\0', MAX_LEXEME_SIZE);
    while (!feof(f)) {
        char c = fgetc(f);
        if (c == EOF) break;
        if (isspace(c)) {
            if (buf_idx > 0) {
                add_to_token_list(list, lexeme_to_token(buf, buf_idx+1, lineno, (start_tok == -1) ? line_idx : start_tok));
                memset(buf, '\0', buf_idx);
                buf_idx = 0;
                start_tok = -1;
            }
            if (c == '\n') {
                buf_idx = 0;
                start_tok = -1;
                ++lineno;
            }
        } else {
            if (start_tok == -1) start_tok = line_idx;
            if (is_op(c) == 0) {
                if (buf_idx > 0) {
                    add_to_token_list(list, lexeme_to_token(buf, buf_idx+1, lineno, start_tok));
                    memset(buf, '\0', buf_idx);
                    buf_idx = 0;
                }
                buf[buf_idx++] = c;
                add_to_token_list(list, lexeme_to_token(buf, buf_idx+1, lineno, line_idx));
                memset(buf, '\0', buf_idx);
                buf_idx = 0;
                start_tok = -1;
            } else {
                buf[buf_idx++] = c;
            }
        }
        ++line_idx;
    }
    if (buf_idx > 0) {
        add_to_token_list(list, lexeme_to_token(buf, buf_idx+1, lineno, start_tok));
    }
    return list;
}


int is_op(char c) {
    if (c == '*' || c == '/' || c == '+' || c == '-' || c == '=' || c == ';' ||
        c == '(' || c == ')' || c == '{' || c == '}' || c == ',' || c == '.') return 0;
    return -1;
}

#ifdef __cplusplus
}
#endif

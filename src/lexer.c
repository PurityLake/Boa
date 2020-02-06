#include "grammar/lexer.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int type;
    char *name;
} _translation_table_entry;

static const _translation_table_entry _trans_table[] = {
    { T_DEF,            "def" },
    { T_IF,             "if" },
    { T_VAR,            "var" },
    { T_BEGIN,          "begin" },
    { T_END,            "end" },
    { T_LPAREN,         "(" },
    { T_RPAREN,         ")" },
    { T_LCURL,          "{" },
    { T_RCURL,          "}" }, 
    { T_COMMA,          "," },
    { T_DOT,            "." },
    { T_MULT,           "*" },
    { T_PLUS,           "+" },
    { T_MINUS,          "-" },
    { T_DIV,            "/" },
    { T_EQ,             "=" },
    { T_SEMI,           ";" },
    { T_IDENT,          "IDENT" },
    { T_INTEGER,        "INTEGER" },
    { T_FLOAT,          "FLOAT" },
    { T_BLOCK,          "BLOCK" },
    { T_VAR_DEC,        "VAR_DEC" },
    { T_FUNC_DEF,       "FUNC_DEF" },
    { T_PARAM_LIST,     "PARAM_LIST" },
    { T_SPLIT,          "SPLIT" }
};

static const size_t TABLE_SIZE = sizeof(_trans_table) / sizeof(_translation_table_entry);

int is_op(char c) {
    if (c == '*' || c == '/' || c == '+' || c == '-' || c == '=' || c == ';' ||
        c == '(' || c == ')' || c == '{' || c == '}' || c == ',' || c == '.') return 1;
    return 0;
}

Token *create_Token(int type, char *value, unsigned int line, unsigned int col) {
    Token *ret = (Token *)malloc(sizeof(Token));
    ret->type = type;
    ret->line = line;
    ret->col = col;
    ret->value = value;
    return ret;
}

void free_Token(Token *token) {
    free(token->value);
    token->value = NULL;
    free(token);
    token = NULL;
}

void free_nostr_Token(Token *token) {
    free(token);
    token = NULL;
}

TokenList *create_TokenList() {
    TokenList *out = (TokenList *)malloc(sizeof(TokenList));
    out->token = NULL;
    out->prev = NULL;
    out->next = NULL;
    return out;
}

void add_to_TokenList(TokenList *list, Token *token) {
    if (list == NULL) return;
    if (list->token == NULL) {
        list->token = token;
        list->next = create_TokenList();
        list->next->prev = list;
    } else {
        TokenList *temp = list;
        while (temp->token != NULL) {
            temp = temp->next;
        }
        temp->token = token;
        temp->next = create_TokenList();
        temp->next->prev = temp;
    }
}

void free_TokenList(TokenList *list) {
    if (list->token != NULL) {
        free_TokenList(list->next);
        free_Token(list->token);
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
            return create_Token(entry.type, resized_lexeme, lineno, col);
        }
    }
    return create_Token(T_IDENT, resized_lexeme, lineno, col);
}

opcode_t str_to_opcode(char *str) {
    _translation_table_entry entry;
    for (int i = 0; i < TABLE_SIZE; ++i) {
        entry = _trans_table[i];
        if (strcmp(str, entry.name) == 0) {
            return entry.type;
        }
    }
    return T_IDENT;
}

char *opcode_to_str(opcode_t opcode) {
    _translation_table_entry entry;
    for (int i = 0; i < TABLE_SIZE; ++i) {
        entry = _trans_table[i];
        if (opcode == entry.type) {
            return entry.name;
        }
    }
    return "UNKNOWN";
}

TokenList *lex_file(FILE *f) {
    TokenList *list = create_TokenList();
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
                add_to_TokenList(list, lexeme_to_token(buf, buf_idx+1, lineno, (start_tok == -1) ? line_idx : start_tok));
                memset(buf, '\0', buf_idx);
                buf_idx = 0;
                start_tok = -1;
            }
            if (c == '\n') {
                ++lineno;
            }
        } else {
            if (start_tok == -1) start_tok = line_idx;
            if (is_op(c)) {
                if (buf_idx > 0) {
                    add_to_TokenList(list, lexeme_to_token(buf, buf_idx+1, lineno, start_tok));
                    memset(buf, '\0', buf_idx);
                    buf_idx = 0;
                }
                buf[buf_idx++] = c;
                add_to_TokenList(list, lexeme_to_token(buf, buf_idx+1, lineno, line_idx));
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
        add_to_TokenList(list, lexeme_to_token(buf, buf_idx+1, lineno, start_tok));
    }
    return list;
}

#ifdef __cplusplus
}
#endif

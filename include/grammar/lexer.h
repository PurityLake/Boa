#ifndef __H_LEXER__
#define __H_LEXER__

#define MAX_LEXEME_SIZE 128
#define MAX_TOKENS_PER_LINE 128

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned opcode_t;

enum {
    T_IDENT = 0,
    
    T_KEYWORDS_START,
    T_DEF, T_IF, T_VAR,
    T_KEYWORDS_END,

    T_OPS_START,
    T_LPAREN, T_RPAREN, T_LCURL, T_RCURL,
    T_COMMA, T_DOT,
    T_MULT, T_PLUS, T_MINUS, T_DIV, T_EQ, T_SEMI,
    T_OPS_END,

    T_BLOCK, T_VAR_DEC
};

typedef struct {
    opcode_t type;
    unsigned int line;
    unsigned int col;
    char *value;
} Token;

typedef struct {
    int type;
    char *name;
} _translation_table_entry;

static const _translation_table_entry _trans_table[] = {
    { T_DEF,    "def" },
    { T_IF,     "if" },
    { T_VAR,    "var" },
    { T_LPAREN, "(" },
    { T_RPAREN, ")" },
    { T_LCURL,  "{" },
    { T_RCURL,  "}" }, 
    { T_COMMA,  "," },
    { T_DOT,    "." },
    { T_MULT,   "*" },
    { T_PLUS,   "+" },
    { T_MINUS,  "-" },
    { T_DIV,    "/" },
    { T_EQ,     "=" },
    { T_SEMI,   ";" }
};

static const size_t TABLE_SIZE = sizeof(_trans_table) / sizeof(_translation_table_entry);

Token  *create_token(int type, char *value, unsigned int line, unsigned int col);
void    free_token(Token **tok);
void    free_token_array(Token ***arr);
Token **lex_line(const char *line_text, unsigned int lineno);

int is_op(char c);

#ifdef __cplusplus
}
#endif

#endif /* __H_LEXER__ */

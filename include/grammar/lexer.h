#ifndef __H_LEXER__
#define __H_LEXER__

#define MAX_LEXEME_SIZE 128

#include <ctype.h>
#include <stdio.h>
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

    T_PARSER_START,
    T_BLOCK, T_VAR_DEC, T_FUNC_DEF, T_PARAM_LIST, T_SPLIT
};

typedef struct {
    opcode_t type;
    unsigned int line;
    unsigned int col;
    char *value;
} Token;

typedef struct _tokenlist {
    Token *token;
    struct _tokenlist *prev, *next;
} TokenList;

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
void    free_token(Token *tok);

TokenList *create_token_list();
void add_to_token_list(TokenList *list, Token *token);
void free_token_list(TokenList *list);

TokenList *lex_file(FILE *filename);

int is_op(char c);

#ifdef __cplusplus
}
#endif

#endif /* __H_LEXER__ */

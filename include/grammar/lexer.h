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
    // identifier
    T_IDENT = 0,
    
    // keywords section
    T_KEYWORDS_START,
    T_DEF, T_IF, T_VAR, T_BEGIN, T_END,
    T_KEYWORDS_END,

    // operators section
    T_OPS_START,
    T_LPAREN, T_RPAREN, T_LCURL, T_RCURL,
    T_COMMA, T_DOT,
    T_MULT, T_PLUS, T_MINUS, T_DIV, T_EQ, T_SEMI,
    T_OPS_END,

    T_LITERALS_START,
    T_INTEGER, T_FLOAT,
    T_LITERALS_END,

    // used by the parser
    T_PARSER_START,
    T_BLOCK, T_VAR_DEC, T_FUNC_DEF, T_PARAM_LIST, T_SPLIT
};

/* Token
 * =================================================================================
 * Basic building block passed on to the parser.
 * type -> the type of the lexeme defined by an enum
 * line -> the line number of the token
 * col -> the column where the token starts
 * value -> the string representation of the lexeme
 */
typedef struct {
    opcode_t type;
    unsigned int line;
    unsigned int col;
    char *value;
} Token;

/* TokenList
 * =================================================================================
 * A doubly linked list that contains a token. This is used by the program to keep
 * an easily transervable list of all the tokens in a file
 * token -> the token of the current index
 * prev -> previous token in the list
 * next -> next token in the list
 */
typedef struct _tokenlist {
    Token *token;
    struct _tokenlist *prev, *next;
} TokenList;

Token *create_Token(int type, char *value, unsigned int line, unsigned int col);
int compare_Token(Token *a, Token *b);
void free_Token(Token *tok);
void free_nostr_Token(Token *tok);

TokenList *create_TokenList();
void add_to_TokenList(TokenList *list, Token *token);
void free_TokenList(TokenList *list);

opcode_t str_to_opcode(char *str);
char *opcode_to_str(opcode_t opcode);

// Takes a file as an argument and tokenises the contents
TokenList *lex_file(FILE *filename);

#ifdef __cplusplus
}
#endif

#endif /* __H_LEXER__ */

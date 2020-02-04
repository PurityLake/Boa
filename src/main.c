#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "boa.h"

int higher_precendence(char a, char b);
int evauluate(Node *node);

int main(int argc, char **argv) {
    if (argc > 1) {
        if (strcmp(argv[1], "--object") == 0) {
            init_GC();
            BoaInteger *a = create_BoaInteger(1);
            BoaInteger *b = create_BoaInteger(2);
            BoaList *list = create_BoaList(32);
            ADD_FUNC(list, a);
            ADD_FUNC(list, b);
            char *value = STR_FUNC(list);
            printf("%s\n", value);
            free(value);
            destroy_BoaList(list);
            destroy_BoaInteger(a);
            destroy_BoaInteger(b);
            cleanup_GC();
        } else if (strcmp(argv[1], "--gc") == 0) {
            init_GC();
            int l = len_GC();
            printf("len before malloc: %d\n", l);
            for (int i = 0; i < 1000; ++i) {
                BoaInteger *bi = (BoaInteger *)malloc_GC(sizeof(BoaInteger));
                bi->base.ref_count = 0;
                bi->value = i;
                if (i % 2 == 0) {
                    INC_REF_COUNT(bi);
                }
            }
            l = len_GC();
            printf("len before sweep: %d\n", l);
            sweep_GC();
            l = len_GC();
            printf("len after sweep: %d\n", l);
            cleanup_GC();
        } else {
            FILE *prog = fopen(argv[1], "r");
            if (prog != NULL) {
                TokenList *list = lex_file(prog);
                set_parser_tokens(list);
                Node *n = parse_list();
                while (n != NULL) {
                    print_Node(n);
                    printf("\n");
                    free_Node(n);
                    n = parse_list();
                }
                free_Node(n);
                free_TokenList(list);
                fclose(prog);
            } else {
                fprintf(stderr, "Failed to open file '%s'\n", argv[1]);
            }
        }
    }
    return 0;
}

int higher_precendence(char a, char b) {
    if (a == b) {
        return 1;
    }
    if (a == '+') {
        if (b == '-') return 1;
        return b == '*' || b == '/';
    } else if (a == '-') {
        if (b == '+') return 1;
        return b == '*' || b == '/';
    } else if (a == '*') {
        if (b == '/') return 1;
        return b == '+' || b == '-';
    } else {
        if (b == '*') return 1;
        return b == '+' || b == '-';
    }
}

int evauluate(Node *node) {
    if (node->left == NULL && node->right == NULL) return atoi(node->token->value);
    int operand_stack[10];
    int *operand = operand_stack;
    char operator_stack[10];
    char *op = operator_stack;

    while (node->right != NULL) { 
        *operand = atoi(node->left->token->value);
        ++operand;
        if (op != operator_stack && higher_precendence(*(op - 1), node->token->value[0])) {
            int result;
            --operand;
            --op;
            switch (*op) {
                case '+':
                    result = (*(operand - 1)) + (*operand);
                    break;
                case '-':
                    result = (*(operand - 1)) - (*operand);
                    break;
                case '*':
                    result = (*(operand - 1)) * (*operand);
                    break;
                case '/':
                    result = (*(operand - 1)) / (*operand);
                    break;
            }
            *operand = result;
            ++operand;
        }
        *op = node->token->value[0];
        ++op;
        node = node->right;
    }
    *operand = atoi(node->token->value);
    while (operand != operand_stack && op != operator_stack) {
        int result;
        --op;
        switch (*op) {
            case '+':
                result = (*(operand - 1)) + (*operand);
                break;
            case '-':
                result = (*(operand - 1)) - (*operand);
                break;
            case '*':
                result = (*(operand - 1)) * (*operand);
                break;
            case '/':
                result = (*(operand - 1)) / (*operand);
                break;
        }
        --operand;
        *operand = result;
    }

    return *operand;
}

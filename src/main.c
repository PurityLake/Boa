#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "boa.h"

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
                    //printf("STORE_POP\t%s %d\n", n->left->left->left->token->value, evauluate(n->left->left->right));
                    printf("%d\n", evauluate(n->left->left->right));
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

int evauluate(Node *node) {
    if (node->left == NULL && node->right == NULL) return atoi(node->token->value);
    int _operands[32];
    int *operands = _operands;

    while (node != NULL) {
        printf("%s          %s\n", node->token->value, opcode_to_str(node->token->type));
        int is_op = node->token->type > T_OPS_START && node->token->type < T_OPS_END;
        if (!is_op) {
            //printf("PUSH_CONST\t%s\n", node->token->value);
            *operands = atoi(node->token->value);
            ++operands;
        } else {
            --operands;
            switch (node->token->type) {
                case T_MULT:
                    //printf("MULT\n");
                    *(operands - 1) = *(operands - 1) * *operands;
                    break;
                case T_DIV:
                    //printf("DIV\n");
                    *(operands - 1) = *(operands - 1) / *operands;
                    break;
                case T_PLUS:
                    //printf("PLUS\n");
                    *(operands - 1) = *(operands - 1) + *operands;
                    break;
                case T_MINUS:
                    //printf("SUB\n");
                    *(operands - 1) = *(operands - 1) - *operands;
                    break;
            }
        }
        node = node->right;
    }
    --operands;
    return *operands;
}

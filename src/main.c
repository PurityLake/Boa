#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "boa.h"

int main(int argc, char **argv) {
    if (argc > 1) {
        if (strcmp(argv[1], "--object") == 0) {
            BoaInteger *i = malloc_BoaInteger(2);
            BoaInteger *j = malloc_BoaInteger(1);
            BoaList *list = malloc_BoaList(32);
            ADD_FUNC(list, i);
            ADD_FUNC(list, j);
            char *value = STR_FUNC(list);
            printf("%s\n", value);
            free(value);
            free_BoaList(list);
            free_BoaInteger(i);
            free_BoaInteger(j);
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
                int i = 0;
                int lineno = 1;
                char line_str[128];

                memset(line_str, '\0', 128);
                while (!feof(prog)) {
                    char c = fgetc(prog);
                    if (c == '\n' || feof(prog)) {
                        if (i > 0) {
                            token **line = lex_line(line_str, lineno);
                            
                            printf("%s\n", line_str);
                            parse_line(line);
                            printf("\n");
                            
                            free_token_array(&line);
                            memset(line_str, '\0', i+1);
                            i = 0;
                        }
                        ++lineno;
                    } else {
                        line_str[i++] = c;
                        if (i >= 128) {
                            fprintf(stderr, "Line overflow\n");
                            break;
                        }
                    }
                }
                fclose(prog);
            } else {
                fprintf(stderr, "Failed to open file '%s'\n", argv[1]);
            }
        }
    }
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "object.h"

int main(int argc, char **argv) {
    if (argc > 1) {
        if (strcmp(argv[1], "--object") == 0) {
            BoaInteger *i = malloc_BoaInteger(2);
            BoaInteger *j = malloc_BoaInteger(1);
            BoaInteger *out = BOA_CAST_OBJ_TO(ADD_FUNC(i, j), BoaInteger);
            printf("%llu\n", getvalue_BoaInteger(out));
            free_BoaInteger(i);
            free_BoaInteger(j);
            free_BoaInteger(out);
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

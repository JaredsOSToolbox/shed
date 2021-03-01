
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define S 5

void strok_test() {
    char* input = "echo --version | grep Brian";

    char* string[256];
    char* buffer = (char*)malloc(BUFSIZ * sizeof(char));
    char** strings = malloc(S * sizeof(char*));
    for (int p = 0; p < S; ++p) {
        strings[p] = (char*)malloc(BUFSIZ * sizeof(char));
    }

    char delimit[] = "|<>";
    size_t delim_size = strlen(delimit);

    int i = 0, j = 0, exported_pos = 0, found = 0;

    while (*input != '\0') {
        for (size_t k = 0; k < delim_size; ++k) {
            if (*input == delimit[k]) {
                switch (*input) {
                    case '|':
                        printf("pipe used\n");
                        break;
                    case '>':
                        printf("output redirection used\n");
                        break;
                    case '<':
                        printf("input redirection used\n");
                        break;
                }
                strings[exported_pos++][++i] = '\0';
                i = 0;
                while (isspace(*input) || *input == delimit[k]) {
                    *input++;
                }
            }
        }
        strings[exported_pos][i++] = *input++;
    }
    /*
     * export these to command_t for processing
    */

    for (int l = 0; l < S; ++l) {
        if (l < exported_pos) {
            printf("%s\n", strings[l]);
        }
        free(strings[l]);
    }
}

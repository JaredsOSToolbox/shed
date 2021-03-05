#include "../includes/strings.h"
#include "../includes/command_t.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char* get_line() {
    /*
     * We want to grab the line from stdin
     */

    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    char* retline = (char*)malloc(MAX_LINE_SIZE * sizeof(char*));

    while ((read = getline(&line, &len, stdin)) != EOF) {
        if (line[0] == 0x0A) {
            free(retline);
            return NULL;
        }
        size_t s_len = strlen(line);
        if (line[s_len - 1] == 0x0A) {
            line[s_len - 1] = '\0';
            break;
        }
    }
    if (strlen(line) == 0) {
        // EOF
        free(retline);
        // pass in garbage bin struct that has pointers to all allocated blocks
        // of memory and automatically free them in this chunk here?
        exit(0);
        /*return NULL;*/
    }

    strcpy(retline, line);
    return retline;
}

/*char** parse_input(char* line) {*/
/*char* token;*/
/*token = strtok(line,)*/
/*}*/

char* strip(char* string, char delimiter) {
    char* stripped = (char*)malloc(sizeof(char*));
    int j = 0;
    for (size_t i = 0; i < strlen(string); ++i) {
        if (string[i] != delimiter) {
            stripped[j++] = string[i];
        }
    }
    stripped[++j] = '\0';
    return stripped;
}

struct command_t** parse_line(char* input) {
    struct command_t** container = malloc(COM_SIZ * sizeof(struct command_t*));

    char** strings = malloc(MAX_STR_COUNT * sizeof(char*));

    for (int p = 0; p < MAX_STR_COUNT; ++p) {
        strings[p] = (char*)malloc(BUFSIZ * sizeof(char));
    }

    char delimit[] = "|<>&";
    size_t delim_size = strlen(delimit);
    
    /*
     * i : indexing position of string that is being copied
     * j : position of command_t in command_t** buffer
     * n : nth string allocated, not exceeding MAX_STR_COUNT
    */

    int i = 0, j = 0, n = 0;

    while (*input != '\0' && j <= COM_SIZ && n <= MAX_STR_COUNT) {
        for (size_t k = 0; k < delim_size; ++k) {
            if (*input == delimit[k]) {
                strings[n++][++i] = '\0';
                i = 0;
                struct command_t* command = command_t_constructor(strings[n-1]);
                switch (*input) {
                    case '|':
                        // signal to current command that it needs to look in
                        // the command_t* buffer as a look ahead here you could
                        // set the output stream?
                        command->pipe_stream = 1;
                         printf("pipe used\n");
                        break;
                    case '>':
                        command->output_stream = 1;
                        printf("output redirection used\n");
                        break;
                    case '<':
                        command->input_stream = 1;
                        printf("input redirection used\n");
                        break;
                    case '&':
                        command->background_process = 1;
                        printf("background process invoked\n");
                        break;
                }
                container[j++] = command;
                while ((isspace(*input) || *input == delimit[k]) && (*input != '\0')) {
                    // advance string pointer further to trim off excess delimiters and whitespace
                    input++;
                }
            }
        }
        strings[n][i++] = *input++;
    }

    // no matches to the above delimiters, just regular command
    if(n == 0 || i > 0){
        strings[n][++i] = '\0';
        struct command_t* comm = command_t_constructor(strings[n]);
        container[j++] = comm;
    }
    printf("current number of commands: %d\n", j);

    // remove any extra strings previously allocated

    /*for(int x = n+2; x < MAX_STR_COUNT; ++x){*/
        /*free(strings[x]);*/
    /*} */
    /*for (int y = j+1; y < COM_SIZ; ++y){*/
        /*free(container[y]);*/
    /*}*/
    container[j+1] = NULL;
    return container;
}


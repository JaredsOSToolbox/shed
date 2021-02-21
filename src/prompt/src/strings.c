#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../includes/strings.h"


char* get_line() {
    /*
     * We want to grab the line from stdin
    */

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    char* retline = (char*)malloc(MAX_LINE_SIZE * sizeof(char*));

    while ((read = getline(&line, &len, stdin)) != EOF) {
        if(line[0] == 0x0A) {
            free(retline);
            return NULL;
        }
        size_t s_len = strlen(line);
        if(line[s_len-1] == 0x0A){ 
            line[s_len-1] = '\0';
            break;
        }
    }
    if(strlen(line) == 0){
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
    for(size_t i = 0; i < strlen(string); ++i){
        if(string[i] != delimiter){
            stripped[j++] = string[i];
        }
    }
    stripped[++j] = '\0';
    return stripped;
}

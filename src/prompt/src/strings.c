#include <stdio.h>
#include <stdlib.h>

#include "../includes/strings.h"


char* get_line() {
    int c;
    char* line = (char*)malloc(MAX_LINE_SIZE * sizeof(char*));
    int i = 0;
    while((c = getchar()) != EOF){
        line[i++] = c;
    }
    line[++i] = '\0';
    return line;
}

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

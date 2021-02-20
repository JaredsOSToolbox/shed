#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "includes/history.h"


const char* INSTITUTION = "California State University Fullerton";
const char* AUTHOR = "Jared Dyreson";
const float VERSION = 1.0;

void version(void){
  printf(
    "Shed Prompt (%s) %f\n"
    "Copyright (C) 2021 Comrade Software Foundation, Inc.\n"
    "MIT License\n"
    "This is free software, and provided as is without warranty\n"
    "Written by %s\n",
  VERSION, INSTITUTION, AUTHOR); 
}

int main(void) {
    int running = 1;

    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int line_number_ = 0;

    struct history* history = history_constructor();
    for(int i = 0; i < 100; i++){
        history_insert(history, "echo hello");
    }
    /*history_insert(history, "ls --version");*/

    /*while(running){*/
        /*printf("shed > ");*/
        /*fflush(stdout);*/
        /*while ((read = getline(&line, &len, stdin)) != EOF && (h < HISTORY_SIZ)) {*/
            /*if(strcmp(line, "!!\n") == 0){*/
                /*if(h == 0){*/
                    /*fprintf(stderr, "no commands in shed history\n");*/
                /*} else {*/
                    /*fflush(stdout);*/
                /*}*/
            /*} else {*/
                /*if(line[0] != '\n'){*/
                    /*printf("adding %s", line);*/
                /*}*/
            /*}*/
            /*printf("shed > ");*/
        /*}*/
        /*running = 0;*/
    /*}*/

    printf("\n");
    /*history_print(history);*/
    history_destructor(history);
    return 0;
}

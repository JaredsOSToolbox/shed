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

void spawn(char* command){
    char* args[MAX_LINE/2 + 1];

    if(command[0] == '\n'){ return; }
    if(strlen(command) > MAX_LINE){
        fprintf(stderr, "command too big\n");
        return;
    }

    // parse command

    char* tok = command;
    int i = 0;

    while((tok = strtok(tok, " ")) != NULL){
        args[i++] = tok;
        tok = NULL;
    }
    --i;
    
    args[i] = strip(args[i], '\n');

    pid_t pid = fork();
    if(pid < 0){
        fprintf(stderr, "fork failed, use a spoon\n");
        return;
    }
    else if(pid == 0){
        int status = execvp(args[0], args);
        if(status == EOF){
            fprintf(stderr, "process cowardly refused to terminate\n");
            exit(1);
        }
    } else{
        wait(NULL);
    }

    free(args[i]);
}

int main(void) {
    int running = 1;

    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int line_number_ = 0;

    char** shed_history = malloc(HISTORY_SIZ * sizeof(char*));
    for(int i = 0; i < HISTORY_SIZ; ++i){
        shed_history[i] = malloc((MAX_LINE + 1) * sizeof(char));
    }
    int h = 0;

    while(running){
        printf("shed > ");
        fflush(stdout);
        while ((read = getline(&line, &len, stdin)) != EOF && (h < HISTORY_SIZ)) {
            if(strcmp(line, "!!\n") == 0){
                if(h == 0){
                    fprintf(stderr, "no commands in shed history\n");
                } else {
                    fflush(stdout);
                    // printf("[INFO] We are using the previous command of %s\n", shed_history[h-1]);
                    int prev = h-1;
                    // strncpy(shed_history[h++], shed_history[prev], strlen(shed_history[prev]));
                    spawn(shed_history[prev]);
                }
            } else {
                if(line[0] != '\n'){
                    printf("adding %s", line);
                    strncpy(shed_history[h++], line, strlen(line)-1);
                    spawn(line);
                }
            }
            printf("shed > ");
        }
        running = 0;
    }
    printf("\n");
    printf("[INFO] Dumping shed history\n");

    for(int i = 0; i < HISTORY_SIZ; ++i){
        printf("[%d] %s\n", i, shed_history[i]);
        free(shed_history[i]);
    }
    free(shed_history);
    return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX_LINE 80

const char* INSTITUTION = "California State University Fullerton";
const char* AUTHOR = "Jared Dyreson";

void version(void){
  printf(
    "Shed Prompt (%s) 1.0\n"
    "Copyright (C) 2021 Comrade Software Foundation, Inc.\n"
    "MIT License\n"
    "This is free software, and provided as is without warranty\n"
    "Written by %s\n",
  INSTITUTION, AUTHOR); 
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

void spawn(char* command){
    char* args[MAX_LINE/2 + 1];

    if(command[0] == '\n'){ return; }
    if(strlen(command) > MAX_LINE){
        fprintf(stderr, "oh fuck it's too big daddy!\n");
        return;
    }

    // parse command

    char* tok = command;
    int i = 0;

    while((tok = strtok(tok, " ")) != NULL){
        args[i++] = tok;
        // printf("%s\n", tok);
        tok = NULL;
    }
    --i;
    
    /*--i;*/
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
    }
    else{
        wait(NULL);
        // printf("Child pid=%d done hath completed\n", pid);
    }

    free(args[i]);
}

int main(void) {
    int running = 1;

    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int line_number_ = 0;

    /*
     * spawing children all over the place, like a salmon
     * ^ this might be kind of graphic Jared...
    */

    while(running){
        printf("shed > ");
        fflush(stdout);
        while ((read = getline(&line, &len, stdin)) != EOF) {
            spawn(line);
            printf("shed > ");
        }
        running = 0;
    }

    free(line);
    return 0;
}

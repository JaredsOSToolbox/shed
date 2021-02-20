#include "../includes/command_t.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

struct command_t* command_t_constructor(char* line) {
    struct command_t* command = (struct command_t*)malloc(sizeof(struct command_t*));
    char* command_path = (char*)malloc(MAX_COMMAND_NAME_SIZ * sizeof(char));
    char** command_args = malloc((MAX_WORDS + 1) * sizeof(char*));

    for(int j = 0; j < MAX_WORDS; ++j){
        command_args[j] = malloc((MAX_COMMAND_NAME_SIZ+1) * sizeof(char));
    }

    int i = 0;

    const char delimiter[2] = " ";
    char* token; 

    token = strtok(line, delimiter);
    while(token != NULL) {
        if(i == 0){
            strncpy(command_path, token, strlen(token));
            strncpy(command_args[i], token, strlen(token));
        } else{
            strncpy(command_args[i], token, strlen(token));
        }
        token = strtok(NULL, delimiter);
        ++i;
    }

    command->command_path = command_path;
    command->arguments = command_args;
    command->argc = i-1;

    // trim off excess data
    // bad design, can be fixed
    /*for(int j = i; j < MAX_WORDS; ++j){ free(command->arguments[j]); }*/
    return command;
}

void command_t_destructor(struct command_t* command){
    free(command->command_path);

    for(int i = 0; i < MAX_WORDS; ++i){ free(command->arguments[i]); }

    free(command->arguments);
    free(command);
}

void command_t_invoke(struct command_t* command) {
    pid_t pid = fork();
    if(pid < 0){
        fprintf(stderr, "fork failed, use a spoon\n");
        return;
    }
    else if(pid == 0){
        int status = execvp(command->command_path, command->arguments);
        if(status == EOF){
            fprintf(stderr, "process cowardly refused to terminate\n");
            exit(1);
        }
    } else{
        wait(NULL);
    }
}

void command_t_print(struct command_t* command) {
    printf("command_path: %s\n", command->command_path);
    printf("command arguments:\n");
    for(int i = 0; i < command->argc; ++i){
        printf("\t[%d]: %s\n", i, command->arguments[i]);
    }
}

#include "../includes/command_t.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

struct command_t* command_t_constructor(char* line) {
    struct command_t* command = (struct command_t*)malloc(sizeof(struct command_t*));
    char* command_path = (char*)malloc(MAX_COMMAND_NAME_SIZ * sizeof(char));
    char** command_args = malloc((MAX_WORDS + 1) * sizeof(char*));

    int i = 0;

    const char delimiter[2] = " ";
    char* token; 

    token = strtok(line, delimiter);
    while((token != NULL)  && ((i - 1) < MAX_WORDS)) {
        if(i == 0){
            strncpy(command_path, token, strlen(token));
            printf("command path yields: %s\n", command_path);
        } 
        command_args[i] = malloc((MAX_COMMAND_NAME_SIZ + 1) * sizeof(char));
        strncpy(command_args[i], token, strlen(token));
        token = strtok(NULL, delimiter);
        ++i;
    }

    command->command_path = command_path;
    command->arguments = command_args;
    command->argc = i;

    return command;
}

void command_t_destructor(struct command_t* command){
    free(command->command_path);

    for(int i = 0; i < command->argc; ++i){ free(command->arguments[i]); }

    free(command->arguments);
    free(command);
}

pid_t spawnChild(char* program, char** arg_list) {
    pid_t ch_pid = fork();
    if (ch_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (ch_pid > 0) {
        return ch_pid;
    } else {
        execvp(program, arg_list);
        perror("execve");
        exit(EXIT_FAILURE);
    }
}

void command_t_invoke(struct command_t* command) {
    pid_t child;
    int wstatus;
    child = spawnChild(command->command_path, command->arguments);
    if(waitpid(child, &wstatus, WUNTRACED | WCONTINUED) == -1){
        perror("waitpid");
        exit(EXIT_FAILURE);
    }

}

/*void command_t_invoke(struct command_t* command) {*/
    /*pid_t pid = fork();*/
    /*int wstatus;*/
    /*if(pid < 0){*/
        /*fprintf(stderr, "fork failed, use a spoon\n");*/
        /*return;*/
    /*}*/
    /*else if(pid == 0){*/
        /*int status = execvp(command->command_path, command->arguments);*/
        /*if(status == -1) {*/
            /*fprintf(stderr, "process cowardly refused to terminate\n");*/
            /*exit(1);*/
        /*}*/
    /*} else {*/
        /*printf("waiting for pid to fininsh\n");*/
        /*if(waitpid(pid, &wstatus, WUNTRACED | WCONTINUED) == EOF){*/
            /*printf("waitpid err\n");*/
            /*exit(1);*/
        /*}*/
    /*}*/
/*}*/

void command_t_print(struct command_t* command) {
    printf("command_path: %s\n", command->command_path);
    printf("command arguments:\n");
    for(int i = 0; i < command->argc; ++i){
        printf("\t[%d]: %s\n", i, command->arguments[i]);
    }
}

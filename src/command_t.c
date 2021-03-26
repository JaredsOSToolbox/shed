#include "../includes/command_t.h"

#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>

struct command_t* command_t_constructor(char* line) {
    struct command_t* command =
        (struct command_t*)malloc(sizeof(struct command_t*));

    char* command_path = (char*)malloc(MAX_COMMAND_NAME_SIZ * sizeof(char));

    char** command_args = malloc((MAX_WORDS + 1) * sizeof(char*));

    /*char* stream_path = (char*)malloc(BUFSIZ * sizeof(char));*/

    int i = 0;

    const char delimiter[2] = " ";
    char* token;

    token = strtok(line, delimiter);
    while ((token != NULL) && ((i - 1) < MAX_WORDS)) {
        if(i == 0){
            command->command_path = strdup(token);
        }
        command_args[i] = malloc((MAX_COMMAND_NAME_SIZ + 1) * sizeof(char));
        strncpy(command_args[i], token, strlen(token));
        token = strtok(NULL, delimiter);
        ++i;
    }

    command->arguments = command_args;
    command->argc = i;

    command->input_stream = 0;        // <
    command->output_stream = 0;       // >
    command->pipe_stream = -1;        // |
    command->background_process = 0;  // &

    return command;
}

void command_t_destructor(struct command_t* command) {
    free(command->command_path);

    for (int i = 0; i < command->argc; ++i) {
        free(command->arguments[i]);
    }

    free(command->arguments);
}

pid_t spawnChild(char* program, char** arg_list, int background) {
    // credit here : https://www.delftstack.com/howto/c/execvp-in-c/
    pid_t ch_pid = fork();
    int status;
    if (ch_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (ch_pid > 0) {
        return ch_pid;
    } 
    else if(ch_pid == 0) {
        execvp(program, arg_list);
        perror("execve");
        exit(EXIT_FAILURE);
    } 
}

void command_t_invoke(struct command_t* command, struct command_t* global_context) {
    int wstatus;
    pid_t child = spawnChild(command->command_path, command->arguments,
                       command->background_process);
    if(command->background_process){
        global_context = command;
    }
    /*command_t_print(global_command);*/
    if (waitpid(child, &wstatus, (command->background_process) ? WNOHANG : WUNTRACED | WCONTINUED) == -1) {
    /*if (waitpid(child, &wstatus, WUNTRACED | WCONTINUED) == -1) {*/
        perror("waitpid");
        exit(EXIT_FAILURE);
    }
    /*global_command = NULL;*/
}

void command_t_print(struct command_t* command) {
    printf("command_path: %s\n", command->command_path);
    printf("command arguments:\n");
    for (int i = 0; i < command->argc; ++i) {
        printf("\t[%d]: %s\n", i, command->arguments[i]);
    }
}

void command_t_set_input_stream(char* path) {
    int fd = open(path, O_RDONLY);
    if (fd == EOF) {
        fprintf(stderr,
                "shed: no such file or directory: %s\n", path);
        exit(EXIT_FAILURE);
    } else {
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
}

void command_t_set_output_stream(char* path) {
    // this is considered stdout
    if (path == NULL) {
        return;
    }
    int fdout = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    dup2(fdout, STDOUT_FILENO);
    /*printf("[DEBUG] This is a test for STDOUT processing\n");*/
    close(fdout);
}

void restore_fd(int current_fd, int destination_fd) {
    /*
     * Restore opened file descriptor to both st
    */

    dup2(current_fd, destination_fd);
    close(current_fd);
}


void run_pipeline(struct command_t** container) {
    int fd[2];
    pid_t pid;
    int fdd = STDIN_FILENO;
    int z = 0;

    printf("saving the old stdout and stdin\n");
    int stdout_old = dup(STDOUT_FILENO);
    int stdin_old = dup(STDIN_FILENO);

    while(container[z] != NULL) {
        pipe(fd);
        if((pid = fork()) == -1){
            perror("fork");
            exit(1);
        } else if(pid == 0){
            dup2(fdd, STDIN_FILENO);
            if(container[z+1] != NULL){
                dup2(fd[1], STDOUT_FILENO);
            }
            close(fd[0]);
            execvp(container[z]->command_path, container[z]->arguments);
            exit(1);
        }
        else {
            wait(NULL);
            close(fd[1]);
            fdd = fd[0];
        }
        ++z;
    }

    restore_fd(stdout_old, STDOUT_FILENO);
    restore_fd(stdin_old, STDIN_FILENO);
}

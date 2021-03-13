#include "../includes/command_t.h"

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
    if (ch_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (ch_pid > 0) {
        return ch_pid;
    } else {
        if (background) {
            printf("sending pid %d into the background\n", ch_pid);
            if(setpgid(ch_pid, 0) != 0){
                perror("setpgid() error");
            }
        }
        execvp(program, arg_list);
        perror("execve");
        exit(EXIT_FAILURE);
    }
}

void command_t_invoke(struct command_t* command) {
    pid_t child;
    int wstatus;
    child = spawnChild(command->command_path, command->arguments,
                       command->background_process);
    if (waitpid(child, &wstatus, WUNTRACED | WCONTINUED) == -1) {
        perror("waitpid");
        exit(EXIT_FAILURE);
    }
}

void command_t_print(struct command_t* command) {
    printf("command_path: %s\n", command->command_path);
    printf("command arguments:\n");
    for (int i = 0; i < command->argc; ++i) {
        printf("\t[%d]: %s\n", i, command->arguments[i]);
    }
    printf("pipe fd %d\n", command->pipe_stream);
}

void command_t_set_input_stream(struct command_t* command, char* path) {
    int fd = open(path, O_RDONLY);
    if (fd == EOF) {
        fprintf(stderr,
                "could not set %s as the input stream; file not found\n", path);
        /*exit(EXIT_FAILURE);*/
    } else {
        command->input_stream = 1;
        dup2(fd, 0);
        close(fd);
    }
}

void command_t_set_output_stream(char* path) {
    // this is considered stdout
    if (path == NULL) {
        printf("using stdout as file stream\n");
        return;
    }
    int fdout = open(path, O_WRONLY | O_CREAT, 0600);
    dup2(fdout, STDOUT_FILENO);
    close(fdout);
}

void restore_stdout(int stdout_fd){
    dup2(stdout_fd, 1);
    close(stdout_fd);
}

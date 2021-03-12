#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "includes/command_t.h"
#include "includes/history.h"
#include "includes/strings.h"

const char* INSTITUTION = "California State University Fullerton";
const char* AUTHOR = "Jared Dyreson";
const float VERSION = 1.0;

#define MAX_FILTH 100
#define MAX_PIPELINE_LEN 10

void version(void) {
    printf(
        "Shed Prompt (%s) %f\n"
        "Copyright (C) 2021 Comrade Software Foundation, Inc.\n"
        "MIT License\n"
        "This is free software, and provided as is without warranty\n"
        "Written by %s\n",
        INSTITUTION, VERSION, AUTHOR);
}

void run_pipeline(struct command_t** container) {
    int fd[2];
    pid_t pid;
    int fdd = 0;
    int z = 0;

    while(container[z] != NULL) {
        pipe(fd);
        if((pid = fork()) == -1){
            perror("fork");
            exit(1);
        } else if(pid == 0){
            dup2(fdd, 0);
            /*if(*(container + 1) != NULL){*/
                /*dup2(fd[1], 1);*/
            /*}*/
            if(container[z+1] != NULL){
                dup2(fd[1], 1);
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
}
int main(int argc, const char* argv[]) {
    int running = 1;

    struct history* history = history_constructor();
    struct command_t** garbage =
        (struct command_t**)malloc(MAX_FILTH * sizeof(struct command_t*));

    struct command_t** pipeline =
        (struct command_t**)malloc(MAX_PIPELINE_LEN * sizeof(struct command_t*));

    int garbage_position = 0;
    int pipe_line_position = 0;

    int is_stdin = 0;

    if (argc > 1 && strcmp(argv[1], "-") == 0) {
        is_stdin = 1;
    }

    while (running && garbage_position < MAX_FILTH) {
        printf("shed > ");
        fflush(stdout);
        char* line =
            get_line();  // CTRL-D will result in massive memory leak. smile :)
        if (line == NULL) {
            continue;
        }

        if (is_stdin) {
            printf("\n");
        }

        if (strcmp(line, "!!") == 0) {
            // find prev command
            if (history_isempty(history)) {
                fprintf(stderr,
                        "could not find previous command; history empty\n");
                continue;
            } else {
                line = history_previous(history);
                printf("previous %s\n", line);
            }
        } else {
            history_insert(history, line);
        }
        if (strcmp(line, "exit()") == 0) {
            break;
        }
        char* copy = strdup(line);
        struct command_t** commands = parse_line(copy);
        int z = 0;
        while(commands[z] != NULL && garbage_position < MAX_FILTH && pipe_line_position < MAX_PIPELINE_LEN) {
            if(commands[z]->pipe_stream && (z + 1 < COM_SIZ)) {
                // add to pipeline for processing
                garbage[garbage_position++] = commands[z];
                pipeline[pipe_line_position++] = commands[z];
            } 
            /*if(commands[z]->change_output){*/
                /*// set it*/
            /*}*/
            else {
                ++z;
                break;
            }
            ++z;
        }

        if(pipe_line_position > 0){
            printf("running pipelinem\n");
            run_pipeline(pipeline);
        }
        /*if(commands[z] != NULL){}*/
    }

    /*
     * for some reason, invoking the destructor in the while/for loop
     * there is an overflow bug in the following iterations (after 1st)
     * but it seems to be fixed when doing a garbage collection at the very end
     * why this works, i have no idea. I truly don't.
     */

    /*for (int k = 0; k < garbage_position; ++k) {*/
        /*command_t_destructor(garbage[k]);*/
    /*}*/

    free(garbage);

    printf("\n");
    history_destructor(history);

    return 0;
}


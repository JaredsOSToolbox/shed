#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "includes/command_t.h"
#include "includes/flag_t.h"
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

    struct flag_t* fl = flag_t_constructor();

    int garbage_position = 0;
    int pipe_line_position = 0;

    int is_stdin = 0;

    int stdout_old = 0;

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
            if(commands[z]->pipe_stream == 1 && (z + 1 < COM_SIZ)) {
                // add to pipeline for processing
                /*command_t_print(commands[z]);*/
                garbage[garbage_position++] = commands[z];
                pipeline[pipe_line_position++] = commands[z];
            } 
            else if(commands[z]->output_stream){
                stdout_old = dup(1);
                command_t_set_output_stream(commands[z+1]->stream_path);
                if(commands[z+2] == NULL && pipe_line_position > 0) {
                    pipeline[pipe_line_position++] = commands[z];
                    run_pipeline(pipeline);
                    /*break;*/
                } else {
                    command_t_invoke(commands[z]);
                }
                flag_t_set_flag(fl, OUTPUT);
            } 
            else if(commands[z]->input_stream) {
                flag_t_set_flag(fl, INPUT);
            }             
            /*else {*/
                /*printf("here!\n");*/
                /*++z;*/
                /*break;*/
            /*}*/
            ++z;
        }

        /*if(pipe_line_position > 0){*/
            /*run_pipeline(pipeline);*/
        
        /*}*/
        // reset output
        // https://stackoverflow.com/questions/11042218/c-restore-stdout-to-terminal
        restore_stdout(stdout_old);
    }
   

    /*
     * for some reason, invoking the destructor in the while/for loop
     * there is an overflow bug in the following iterations (after 1st)
     * but it seems to be fixed when doing a garbage collection at the very end
     * why this works, i have no idea. I truly don't.
     */

    for (int k = 0; k < garbage_position; ++k) {
        command_t_destructor(garbage[k]);
    }

    free(garbage);

    printf("\n");
    history_destructor(history);

    return 0;
}


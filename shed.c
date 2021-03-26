#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

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

void harvester_of_sorrow(struct command_t** commands, int position,
                         struct flag_t* flags, struct history* hist) {
    /*
     * Name this band, album and year and you get a free cookie :)
     * Garbage collection that can happen dynamically in these two conditions:
     * 1) End of loop
     * 2) Ctrl-D is pressed, invoking immediate termination
     */

    for (int k = 0; k < position; ++k) {
        /*
         * Remove all commands allocated
        */
        command_t_destructor(commands[k]);
    }
    /*
     * Free container pointer
    */
    free(commands);

    /*
     * Recursively release all strings created for history
     * along with the history
    */
    history_destructor(hist);
    printf("\n");
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
    int stdin_old = 0;

    if (argc > 1 && strcmp(argv[1], "-") == 0) {
        is_stdin = 1;
    }

    /*signal(SIGCHLD, catch);*/
    while (running && garbage_position < MAX_FILTH) {
        printf("shed > ");
        fflush(stdout);

        char* line =
            get_line();

        if (strcmp(line, "EOF") == 0) {
            harvester_of_sorrow(garbage, garbage_position, fl, history);
            return 0;
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
                garbage[garbage_position++] = commands[z];
                pipeline[pipe_line_position++] = commands[z];
            } 
            else if((commands[z]->output_stream || commands[z]->input_stream) && !get_flag(fl, BACKGROUND)){
                char* output = commands[z+1]->output_stream_path;
                char* input = commands[z+1]->input_stream_path;

                stdout_old = dup(STDOUT_FILENO);
                stdin_old = dup(STDIN_FILENO);

                if(commands[z]->output_stream && output != NULL){
                    flag_t_set_flag(fl, OUTPUT);
                    command_t_set_output_stream(output);
                } 
                else {
                    flag_t_set_flag(fl, INPUT);
                    if(input != NULL){
                        command_t_set_input_stream(input);
                    }
                }
                
                if(pipe_line_position > 0 && !get_flag(fl, INPUT)) {
                    pipeline[pipe_line_position++] = commands[z];
                    run_pipeline(pipeline);
                    pipe_line_position = 0;
                }
                if(!(get_flag(fl, INPUT) || get_flag(fl, OUTPUT))){
                    command_t_invoke(commands[z]);
                }
                /*else {*/
                /*}*/

            } 
            else if(commands[z]->background_process){
                flag_t_set_flag(fl, BACKGROUND);
            }
            ++z;
        }

        if(pipe_line_position > 0){
            if(get_flag(fl, BACKGROUND)){
                printf("needing to put this entire pipeline into the background\n");
            }
            run_pipeline(pipeline);
        } else {
            command_t_invoke(commands[0]);
        }

        // reset output for both stdin and stdout
        // https://stackoverflow.com/questions/11042218/c-restore-stdout-to-terminal
        if(get_flag(fl, INPUT)) {
            restore_fd(stdin_old, STDIN_FILENO);
        }
        if(get_flag(fl, OUTPUT)) {
            restore_fd(stdout_old, STDOUT_FILENO);
        }
        clear_flags(fl);
    }
   

    /*
     * for some reason, invoking the destructor in the while/for loop
     * there is an overflow bug in the following iterations (after 1st)
     * but it seems to be fixed when doing a garbage collection at the very end
     * why this works, i have no idea. I truly don't.
     */

    harvester_of_sorrow(garbage, garbage_position, fl, history);

    return 0;
}


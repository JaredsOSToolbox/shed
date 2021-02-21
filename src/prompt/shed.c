#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "includes/history.h"
#include "includes/strings.h"
#include "includes/command_t.h"


const char* INSTITUTION = "California State University Fullerton";
const char* AUTHOR = "Jared Dyreson";
const float VERSION = 1.0;

#define MAX_FILTH 100

void version(void){
  printf(
    "Shed Prompt (%s) %f\n"
    "Copyright (C) 2021 Comrade Software Foundation, Inc.\n"
    "MIT License\n"
    "This is free software, and provided as is without warranty\n"
    "Written by %s\n",
  INSTITUTION, VERSION, AUTHOR); 
}

int main(void) {
    int running = 1;

    struct history* history = history_constructor();
    struct command_t** garbage = (struct command_t**)malloc(MAX_FILTH * sizeof(struct command_t*));
    int garbage_position = 0;

    /*for(int i = 0; i < 2; ++i){*/

        /*char* line =  get_line();*/
        /*printf("got %s\n", line);*/
        /*history_insert(history, line);*/
        /*struct command_t* command = command_t_constructor(line);*/
        /*command_t_print(command);*/
        /*command_t_invoke(command);*/

        /*garbage[j++] = command;*/

        /*command_t_destructor(command);*/
        /*free(line);*/
    /*}*/

    while(running && garbage_position < MAX_FILTH){
        printf("shed > ");
        fflush(stdout);
        char* line =  get_line(); // CTRL-D will result in massive memory leak. smile :)
        if(line == NULL){ continue; }

        if(strcmp(line, "!!") == 0){
            // find prev command
            if(history_isempty(history)) {
                fprintf(stderr, "could not find previous command; history empty\n");
                continue;
            } else{
                line = history_previous(history);
                printf("previous %s\n", line);
            }
        } else {
            history_insert(history, line);
        }
        if(strcmp(line, "exit()") == 0) { break; }
        printf("using %s as the command\n", line);
        struct command_t* command = command_t_constructor(line);
        command_t_invoke(command);
        garbage[garbage_position++] = command;
        /*command_t_destructor(command);*/
    }

    /*
     * for some reason, invoking the destructor in the while/for loop
     * there is an overflow bug in the following iterations (after 1st)
     * but it seems to be fixed when doing a garbage collection at the very end
     * why this works, i have no idea. I truly don't.
    */

    for(int k = 0; k < garbage_position; ++k){
        command_t_destructor(garbage[k]);
    }
    free(garbage);

    printf("\n");
    history_print(history);
    history_destructor(history);
    return 0;
}

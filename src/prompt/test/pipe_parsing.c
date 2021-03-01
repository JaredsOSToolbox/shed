
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main(){
    char string[] = "cat scores | grep line | sort";

    const char delimiter[2] = "|";

    char* token;
    token = strtok(string, delimiter);

    char* commands[10];
    int i = 0;

    while(token != NULL){
        while(*token == ' '){ token++; } // trim leading white space
        commands[i++] = strdup(token);
        token = strtok(NULL, delimiter);
    }
    for(int j = 0; j < i; ++j){
        printf("%s\n", commands[j]);
    }

    int element_count = i+1;
    int pipe_count = i-1;
    int pipe_position = 0;

    int pipes[element_count];
    printf("%d\n", pipe_count);

    pipe(pipes);
    pipe(pipes + pipe_count);

    /*
     * we have a specific formula for two pipes
     * goal is to expand it for N amount of pipes
    */

    if(fork() == 0){
        dup2(pipes[pipe_position + 1], 0);
        /*
         * close all the pipes
        */
        for(int p = 0; p < element_count; ++p){ close(pipes[p]); }
    }

    return 0;
}

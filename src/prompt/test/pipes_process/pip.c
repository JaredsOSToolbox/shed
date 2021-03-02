#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void func(int* pipes, int start, int end, int c, char* dummy_command) {
    if(start == end){
        return;
    }
    int current = 0, behind = 0, ahead = 0;
    if(start % 2  == 0 && (start + 1 < end)){
        // we are only interested in reading from stdin/stdout
        current = start+1;
    } else {
        current = start;
        behind = current - 1;
        ahead = current + 1;
    }
    pid_t ch_pid = fork();

    if (ch_pid == 0 && current > 0) {
        // replace cat's stdout with write part of 1st pipe

        dup2(pipes[current], 1);

        // close all pipes (very important!); end we're using was safely copied
        //
        for(int i = 0; i < c; ++i){
            close(pipes[i]);
        }

        printf("running %s\n", dummy_command);
    } 
    else if(ch_pid == 0 && (behind > 0 && ahead > 0)) {
        dup2(pipes[behind], 0);
        dup2(pipes[ahead], 1);

        for(int i = 0; i < c; ++i){
            close(pipes[i]);
        }
        // should have been able to peel off next command
        printf("sub pipe running %s\n", dummy_command);
    }
    func(pipes, start+1, end, c, dummy_command);
}

int main(){
    int status;
    int i;

    int pipes[4];
    pipe(pipes); // sets up 1st pipe
    pipe(pipes + 2); // sets up 2nd pipe

    int begin = 0;
    int end = 3;
    int count = (end + begin + 1); // 1 more than we have commands

    /*func(pipes, begin, end, count, "echo hello");*/
    
    for(int k = 0; k < end; ++k) {
        pid_t ch_pid = fork();
        int current = k;
        if(ch_pid == 0){
            printf("here\n");
        }
    }
    for(i = 0; i < end; ++i){
        printf("waiting for %d\n", i);
        wait(&status);
    }
    return 0;
}

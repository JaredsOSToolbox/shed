#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
    pid_t pid = fork();
    if(pid < 0){
        fprintf(stderr, "fork failed, use a spoon\n");
        return 1;
    }
    else if(pid == 0){
        execlp("/bin/ls", "ls", NULL);
        printf("LINE J\n");
    }
    else{
        wait(NULL);
        printf("Child pid=%d done hath completed\n", pid);
    }
    return 0;
}

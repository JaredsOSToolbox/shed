
/* signal4: parent creates multiple child processes and detects
   their termination via a signal 
   Each child prints an "I'm the child" message and exits after n seconds,
   where n is the sequence in which it was forked. 

   Paul Krzyzanowski
*/

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUMPROCS 1 /* number of processes to fork */
#define SLEEPY_TIME 1000
int nprocs;        /* number of child processes */

char program[] = "sleep";
char* args[] = {
    "sleep",
    "2",
    NULL
};

int main(int argc, char **argv) {
    void catch (int);     /* signal handler */
    void child(int n);    /* the child calls this */
    void parent(int pid); /* the parent calls this */
    int pid;              /* process ID */
    int i;

    signal(SIGCHLD, catch); /* detect child termination */

    for (i = 0; i < NUMPROCS; i++) {
        switch (pid = fork()) {
            case 0:       /* a fork returns 0 to the child */
                child(i); /* child() never returns; the function exits */
                break;
            case -1: /* something went wrong */
                perror("fork");
                exit(1);
            default:      /* parent just loops to create more kids */
                nprocs++; /* count # of processes that we forked */
                break;
        }
    }
    /*printf("parent: going to sleep\n");*/

    /* do nothing forever; remember that a signal wakes us out of sleeep */
    printf("> ");
    char x = '\0';
    while (nprocs != 0 && x != EOF) {
        x = getchar();
        printf("%c\n", x);
    }
    printf("parent: exiting\n");
    printf("[1] + %d done {PARENT}\n", getpid());
    exit(0);
}

void child(int n) {
    /*printf("\tchild[%d]: child pid=%d, sleeping for %d seconds\n", n, getpid(),*/
           /*n);*/
    execvp(program, args);
    /*printf("\tchild[%d]: I'm exiting\n", n);*/
    /*exit(100 + n); [> exit with a return code of 100+n <]*/
}

void catch (int snum) {
    int pid;
    int status;

    pid = wait(&status);
    printf("[1] + %d done \t%s\n", pid,
           program);
    nprocs--;
    signal(SIGCHLD, catch); /* reset the siganl (for SunOS) */
}

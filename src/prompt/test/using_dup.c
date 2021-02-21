#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// sourced here : https://www.tek-tips.com/viewthread.cfm?qid=1224469
// Another source  : https://linuxhint.com/dup2_system_call_c/
// More complete example : http://gunpowder.cs.loyola.edu/~jglenn/702/S2005/Examples/dup2.html

int test(){
    int fdout = open("file.txt",O_WRONLY | O_CREAT, 0600);
    switch (fork()) {
      case -1:
             //error;
      case 0:
        dup2(fdout, STDOUT_FILENO);
        close(fdout);
        execlp("ls", "ls", NULL);
    }
}

int redirect_stdout(const char* command, char* arguments[], char* path) {
    int fdout = open(path, O_WRONLY | O_CREAT, 0600);
    switch(fork()){
        case EOF:
            fprintf(stderr, "woops\n");
        case 0:
            dup2(fdout, STDOUT_FILENO);
            close(fdout);
            execlp(command, *arguments);
    }
}

int redirect_program_input(const char* command, char* arguments[], char* path) {
    int fdout = open(path, O_WRONLY | O_CREAT, 0600);
    switch(fork()){
        case EOF:
            fprintf(stderr, "woops\n");
        case 0:
            dup2(fdout, STDIN_FILENO);
            close(fdout);
            execlp(command, *arguments);
    }
}

int main (void) {
    // ls > file.txt
    /*
     * whlie parsing string
     * if token == >
    */
    /*test();*/
    char* ls_args[] = {
        "ls",
        "--version",
        NULL
    };
    
    char* sort_args[] = {
        "sort",
        "-u",
        NULL
    };
    int r = redirect_stdout("ls", ls_args, "file.txt");
    int k = redirect_program_input("sort", sort_args, "file.txt");
    return 0;
}

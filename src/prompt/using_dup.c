#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// sourced here : https://www.tek-tips.com/viewthread.cfm?qid=1224469
// Another source  : https://linuxhint.com/dup2_system_call_c/
// More complete example : http://gunpowder.cs.loyola.edu/~jglenn/702/S2005/Examples/dup2.html

#define MAX_LINE_SIZE 100
char* get_line() {
    int c;
    char* line = (char*)malloc(MAX_LINE_SIZE * sizeof(char*));
    int i = 0;
    while((c = getchar()) != EOF){
        line[i++] = c;
    }
    line[++i] = '\0';
    return line;
}

/*int test(){*/
    /*int fdout = open("file.txt",O_WRONLY | O_CREAT, 0600);*/
    /*switch (fork()) {*/
      /*case -1:*/
             /*//error;*/
      /*case 0:*/
        /*dup2(fdout, STDOUT_FILENO);*/
        /*close(fdout);*/
        /*execlp("ls", "ls", NULL);*/
    /*}*/
/*}*/

int main (void) {
  // test();
  char* line = get_line();
  printf("%s\n", line);
  free(line);
  return 0;
}

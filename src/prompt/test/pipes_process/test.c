#include <stdio.h>
#include <stdlib.h>

struct pipe_t {
  int* pipefds;  // pipe file descriptors
  int size;      // amount of file descriptors
  int n;         // current index
};

struct pipe_t* pipe_t_constructor(int);
void pipe_t_destructor(struct pipe_t*);
void pipe_t_clear(struct pipe_t*);

struct pipe_t* pipe_t_constructor(int s) {
  struct pipe_t* pipe_ = (struct pipe_t*)malloc(sizeof(struct pipe_t*));
  int* fds = malloc(s * sizeof(int));
  for (int i = 0; i < s; ++i) {
    if (i % 2 == 0) {
      // set up every other element
      pipe(fds[i]);
    }
  }
  pipe_->size = s;
  pipe_->pipefds = fds;
  pipe_->n = 0;
  return pipe_;
}

void pipe_t_clear(struct pipe_t* pipe) {
  while (pipe != NULL) {
    close(pipe++);
  }
}

void pipe_t_destructor(struct pipe_t* p){
  free(p->pipefds);
  free(p);
}

void test(){
  int status;
  int i;

  // arguments for commands; your parser would be responsible for
  // setting up arrays like these

  char *cat_args[] = {"uname", "-a", NULL};
  char *grep_args[] = {"grep", "Linux", NULL};
  char *cut_args[] = {"cut", "-b", "1-10", NULL};

  // make 2 pipes (cat to grep and grep to cut); each has 2 fds

  int pipes[4];
  pipe(pipes); // sets up 1st pipe
  pipe(pipes + 2); // sets up 2nd pipe

  // we now have 4 fds:
  // pipes[0] = read end of cat->grep pipe (read by grep)
  // pipes[1] = write end of cat->grep pipe (written by cat)
  // pipes[2] = read end of grep->cut pipe (read by cut)
  // pipes[3] = write end of grep->cut pipe (written by grep)

  // Note that the code in each if is basically identical, so you
  // could set up a loop to handle it.  The differences are in the
  // indicies into pipes used for the dup2 system call
  // and that the 1st and last only deal with the end of one pipe.

  // fork the first child (to execute cat)
  
  if (fork() == 0)
    {
      // replace cat's stdout with write part of 1st pipe

      dup2(pipes[1], 1);

      // close all pipes (very important!); end we're using was safely copied

      close(pipes[0]);
      close(pipes[1]);
      close(pipes[2]);
      close(pipes[3]);

      execvp(*cat_args, cat_args);
    }
  else
    {
      // fork second child (to execute grep)

      if (fork() == 0)
  {
    // replace grep's stdin with read end of 1st pipe
    
    dup2(pipes[0], 0);

    // replace grep's stdout with write end of 2nd pipe

    dup2(pipes[3], 1);

    // close all ends of pipes

    close(pipes[0]);
    close(pipes[1]);
    close(pipes[2]);
    close(pipes[3]);

    execvp(*grep_args, grep_args);
  }
      else
  {
    // fork third child (to execute cut)

    if (fork() == 0)
      {
        // replace cut's stdin with input read of 2nd pipe

        dup2(pipes[2], 0);

        // close all ends of pipes

        close(pipes[0]);
        close(pipes[1]);
        close(pipes[2]);
        close(pipes[3]);

        execvp(*cut_args, cut_args);
      }
  }
    }
      
  // only the parent gets here and waits for 3 children to finish
  
  close(pipes[0]);
  close(pipes[1]);
  close(pipes[2]);
  close(pipes[3]);

  for (i = 0; i < 3; i++)
    wait(&status);

}

void new_test() {
  int n = 4;  // dynamically generate this number when processing container

  int status;  // status of the child process
  int i;       // current index of child process

  struct pipe_t* pipe = pipe_t_constructor(n);

  if (fork() == 0) {
      // replace cat's stdout with write part of 1st pipe

      dup2(pipes[1], 1);

      // close all pipes (very important!); end we're using was safely copied

      close(pipes[0]);
      close(pipes[1]);
      close(pipes[2]);
      close(pipes[3]);

      execvp(*cat_args, cat_args);
    }
  else
  pipe_t_destructor(pipe);
}

int main(int argc, char **argv) {
  new_test();
  return 0;
}

#include "../includes/pipe_t.h"

struct pipe_t* pipe_t_constructor(int s) {
  struct pipe_t* pipe = (struct pipe_t*)malloc(sizeof(struct pipe_t*));
  int* fds = malloc(s * sizeof(int));
  for(int i = 0; i < s; ++i){
    if(i % 2 == 0){
      // set up every other element
      pipe(pipes[i]);
    }
  }
  pipe->size = s;
  pipe->pipefds = fds;
  pipe->n = 0;
  return pipe;
}

void pipe_t_clear(pipe_t* pipe) {
  while (*pipe != NULL) {
    close(*pipe++);
  }
}


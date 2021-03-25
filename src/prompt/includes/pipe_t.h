#pragma once
struct pipe_t {
  int* pipefds;  // pipe file descriptors
  int size;      // amount of file descriptors
  int n;         // current index
};

struct pipe_t* pipe_t_constructor(int);
void pipe_t_clear(pipe_t*);

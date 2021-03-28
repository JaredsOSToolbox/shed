#pragma once
#include <stdbool.h>

#define MAX_PIPELINE_LEN 10

struct pipe_t {
  struct command_t** pipe;
  int length;
  int pipe_position;
};

struct pipe_t* pipe_t_constructor(struct command_t**);
void pipe_t_destructor(struct pipe_t*);

void pipe_t_set_position(struct pipe_t*, int);
void pipe_t_reset_position(struct pipe_t*);
void pipe_t_update_size(struct pipe_t*);

bool pipe_t_is_empty(struct pipe_t*);

void pipe_t_clear(struct pipe_t*);
void pipe_t_print(struct pipe_t*);

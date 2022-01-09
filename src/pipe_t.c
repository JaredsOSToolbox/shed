#include "../includes/pipe_t.h"

#include <stdio.h>
#include <stdlib.h>

#include "../includes/command_t.h"

struct pipe_t* pipe_t_constructor(struct command_t** command_list) {
  struct pipe_t* p = (struct pipe_t*)malloc(sizeof(struct pipe_t));

  p->pipe = command_list;
  p->length = 0;

  pipe_t_update_size(p);

  p->pipe_position = 0;
  return p;
}

void pipe_t_destructor(struct pipe_t* p) {
  pipe_t_clear(p);
  free(p);
}

void pipe_t_set_position(struct pipe_t* p, int position) {
  p->pipe_position = position;
}

void pipe_t_reset_position(struct pipe_t* p) { pipe_t_set_position(p, 0); }

void pipe_t_update_size(struct pipe_t* p) {
  struct command_t** c_list_begin = p->pipe;
  while (c_list_begin++ != NULL) {
    p->length++;
  }
}

bool pipe_t_is_empty(struct pipe_t* p) {
  return ((p->length == 0) && (p->pipe_position == 0) && (p->pipe == NULL));
}

void pipe_t_clear(struct pipe_t* p) {
  p->pipe = NULL;
  p->length = p->pipe_position = 0;
}

void pipe_t_print(struct pipe_t* p) {
  for (int i = 0; i < p->length; ++i) {
    if (i + 1 >= p->length) {
      printf("%s\n", p->pipe[i]->command_path);
    } else {
      printf("%s | ", p->pipe[i]->command_path);
    }
    /*command_t_print(p->pipe[i]);*/
  }
}

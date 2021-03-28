#include "../includes/pipe_t.h"
#include "../includes/command_t.h"

#include <stdio.h>
#include <stdlib.h>

struct pipe_t* pipe_t_constructor(struct command_t** command_list){
    struct pipe_t* p = (struct pipe_t*)malloc(sizeof(struct pipe_t));

    p->pipe = command_list;

    struct command_t** c_list_begin = command_list;
    while(c_list_begin++ != NULL){
        p->length++;
    }
    
    p->pipe_position = 0;
    return p;
}

void pipe_t_destructor(struct pipe_t* p) {
    free(p);
}

void pipe_t_set_position(struct pipe_t* p, int position) {
    p->pipe_position = position;
}

void pipe_t_reset_position(struct pipe_t* p){
    pipe_t_set_position(p, 0);
}


void pipe_t_print(struct pipe_t* p){
    for(int i = 0; i < p->length; ++i){
        if(i+1 >= p->length){
            printf("%s\n", p->pipe[i]->command_path);
        } else {
            printf("%s | ", p->pipe[i]->command_path);
        }
        /*command_t_print(p->pipe[i]);*/
    }
}

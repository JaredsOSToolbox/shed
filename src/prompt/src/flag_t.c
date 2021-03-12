#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../includes/flag_t.h"

struct flag_t *flag_t_constructor() {
    struct flag_t *flags = (struct flag_t*)malloc(sizeof(struct flag_t));
    flags->size = SIZE;
    bool *container = malloc(flags->size * sizeof(bool));
    flags->container = container;
    return flags;
}

void flag_t_destructor(struct flag_t* flag){
    free(flag->container);
    free(flag);
}

void flag_t_set_flag(struct flag_t* flags, int flag_pos){
    for(int i = 0; i < flags->size; ++i){
        flags->container[i] = (flag_pos == i) ? true : false;
    }
}

void set_flag(bool *flags, int flag){
    // in our case, we want to turn off the other ones
    for(int i = 0; i < SIZE; ++i){
        flags[i] = (flag == i) ? true : false;
    }
}

bool get_flag(struct flag_t *flags, int flag){
    return flags->container[flag];
}

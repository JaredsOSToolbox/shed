#pragma once
#include <stdbool.h>
#include <stdlib.h>

#define MAX_LINE 80
#define HISTORY_SIZ 10

struct history {
  int length;
  int position;
  char** content;
};

// typedef struct history history;

struct history* history_constructor(void);

void history_content_destructor(struct history*);
void history_destructor(struct history*);

void history_print(struct history*);
void history_insert(struct history*, char*);
void history_resize(struct history*);

char* history_previous(struct history*);
bool history_isempty(struct history*);

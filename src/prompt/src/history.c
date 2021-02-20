#include <stdio.h>
#include <stdlib.h>

#include "../includes/history.h"

struct history* history_constructor(){
    struct history* hist = (struct history*)malloc(sizeof(struct history*));
    char** shed_history = malloc(HISTORY_SIZ * sizeof(char*));
    for(int i = 0; i < HISTORY_SIZ; ++i) {
      shed_history[i] = malloc((MAX_LINE + 1) * sizeof(char));
    }
    hist->content = shed_history;
    hist->position = 0;
    hist->length = HISTORY_SIZ;
    return hist;
}

void history_content_destructor(struct history* history){
  for(int i = 0; i < history->length; ++i){
    free(history->content[i]);
  }
  free(history->content);
}

void history_destructor(struct history* history){
    history_content_destructor(history);
    free(history);
}
void history_print(struct history* history){
  for(int i = 0; i < history->length; ++i){
    printf("[%d] %s\n", i, history->content[i]);
  }
}

void history_insert(struct history* history, char* command){
    printf("calling history_insert with message of %s\n", command);
    if(history->position >= history->length) {
        printf("position (%d) and len (%d)\n", history->position, history->length);
        /*
         * We are going to dynamically resize this array
         * This brings me back to 131, when you made us code vectors from scratch
         * fun. times. yep
        */

        printf("printing the content of history with size (%d) before resizing\n", history->length);
        history_print(history);

        int new_size = history->length * 2;
        char** shed_history = malloc(new_size * sizeof(char*));
        for(int i = 0; i < new_size; ++i) {
            shed_history[i] = malloc((MAX_LINE + 1) * sizeof(char));
            if(i < history->length) {
                strncpy(shed_history[i], 
                        history->content[i], 
                        strlen(history->content[i]
                ));
            }
        }

        history_content_destructor(history);
        history->content = shed_history;
        history->position = history->length;
        history->length = new_size;

    }

    strncpy(
        history->content[history->position++],
        command,
        strlen(command)
    );
    printf("printing the content of history with size (%d)\n", history->length);
    history_print(history);
    /*history->content[history->position++] = strncpy(c)*/

    /*history->contents*/
}

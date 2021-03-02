#define MAX_WORDS 100
#define MAX_COMMAND_NAME_SIZ 50

#include <stdbool.h>

struct command_t {
  char* command_path;
  char** arguments;
  int argc;

  int input_stream;  // is command reading from another command? (have either of
                     // hese been modified)
  int output_stream;  // is command outputting to stdout or another file stream
  int pipe_stream;
  int background_process;
};

struct command_t* command_t_constructor(char*);
void command_t_destructor(struct command_t*);
void command_t_invoke(struct command_t*);
void command_t_print(struct command_t*);

void command_t_set_output_stream(struct command_t*, char*);
void command_t_set_input_stream(struct command_t*, char*);
void command_t_set_background_process(struct command_t*); // cat prog.c &
int command_t_set_pipe_stream(struct command_t*, struct command_t*);

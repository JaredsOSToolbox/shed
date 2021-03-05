#define MAX_WORDS 100
#define MAX_COMMAND_NAME_SIZ 50

#pragma once
#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>

struct command_t {
  char* command_path;
  char** arguments;
  int argc;

  int input_stream;  // is command reading from another command? (have either of
                     // hese been modified)
  int output_stream;  // is command outputting to stdout or another file stream
  
  pid_t current_pipe_pid;
  pid_t previous_pipe_pid;
  int pipe_stream; // using pipe?

  bool has_next_instruction;

  int background_process;
};

struct command_t* command_t_constructor(char*);
void command_t_destructor(struct command_t*);
void command_t_invoke(struct command_t*);
void command_t_print(struct command_t*);

void command_t_set_output_stream(struct command_t*, char*);
void command_t_set_input_stream(struct command_t*, char*);
void command_t_set_background_process(struct command_t*); // cat prog.c &
void command_t_set_pipe_stream(struct command_t*);

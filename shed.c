#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "includes/command_t.h"
#include "includes/flag_t.h"
#include "includes/history.h"
#include "includes/pipe_t.h"
#include "includes/strings.h"

const char* INSTITUTION = "California State University Fullerton";
const char* AUTHOR = "Jared Dyreson";
const float VERSION = 1;

#define MAX_FILTH 100

void version(void) {
  printf(
      "Shed Prompt (%s) %.2f\n"
      "Copyright (C) 2021 Comrade Software Foundation, Inc.\n"
      "MIT License\n"
      "This is free software, and provided as is without warranty\n"
      "Written by %s\n",
      INSTITUTION, VERSION, AUTHOR);
}

void harvester_of_sorrow(struct command_t** commands, int position,
                         struct flag_t* flags, struct history* hist) {
  /*
   * Name this band, album and year and you get a free cookie :)
   * Garbage collection that can happen dynamically in these two conditions:
   * 1) End of loop
   * 2) Ctrl-D is pressed, invoking immediate termination
   */

  flag_t_destructor(flags);

  for (int k = 0; k < position; ++k) {
    /*
     * Remove all commands allocated
     */
    command_t_destructor(commands[k]);
  }
  /*
   * Free container pointer
   */
  free(commands);

  /*
   * Recursively release all strings created for history
   * along with the history
   */
  history_destructor(hist);
  printf("\n");
}

int main(int argc, const char* argv[]) {
  /*if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-v") == 0) {*/
  /*version();*/
  /*return 0;*/
  /*}*/

  int running = 1;

  struct history* history = history_constructor();
  struct command_t** garbage =
      (struct command_t**)malloc(MAX_FILTH * sizeof(struct command_t*));

  struct command_t** pipeline =
      (struct command_t**)malloc(MAX_PIPELINE_LEN * sizeof(struct command_t*));

  struct flag_t* fl = flag_t_constructor();

  int garbage_position = 0;
  int pipe_line_position = 0;

  int is_stdin = 0;

  int stdout_old = 0;
  int stdin_old = 0;

  if (argc > 1 && strcmp(argv[1], "-") == 0) {
    is_stdin = 1;
  }

  /*signal(SIGCHLD, catch);*/
  while (running && garbage_position < MAX_FILTH) {
    printf("shed > ");
    fflush(stdout);

    char* line = get_line();

    if (strcmp(line, "EOF") == 0) {
      harvester_of_sorrow(garbage, garbage_position, fl, history);
      return 0;
    }
    if (is_stdin) {
      printf("\n");
    }

    if (strcmp(line, "!!") == 0) {
      // find prev command
      if (history_isempty(history)) {
        fprintf(stderr, "could not find previous command; history empty\n");
        continue;
      } else {
        line = history_previous(history);
      }
    } else {
      history_insert(history, line);
    }

    if (strcmp(line, "exit()") == 0) {
      break;
    }
    char* copy = strdup(line);
    struct command_t** commands = parse_line(copy);
    struct command_t*** c_list_begin = &commands;
    int length = 0;
    char *input, *output;

    while (*c_list_begin++ != NULL) {
      ++length;
    }

    for (int i = 0; i < length - 1; ++i) {
      if (commands[i]->pipe_stream) {
        // add to pipeline for processing
        garbage[garbage_position++] = commands[i];
        pipeline[pipe_line_position++] = commands[i];
      }

      if (commands[i]->output_stream && !get_flag(fl, BACKGROUND)) {
        if (i + 1 <= length) {
          output = commands[i + 1]->command_path;
        }
        stdout_old = dup(STDOUT_FILENO);
        flag_t_set_flag(fl, OUTPUT);
        command_t_set_output_stream(output);
      }

      if (commands[i]->input_stream && !get_flag(fl, BACKGROUND)) {
        // this further thickens the plot
        // FIXME : proper command processing in src/strings.c
        /*if(i+1 <= length){*/
        /*input = commands[i+1]->command_path;*/
        /*}*/
        input = commands[i + 1]->command_path;
        if (commands[i]->output_stream) {
          output = commands[i]->output_stream_path;
        }

        stdin_old = dup(STDIN_FILENO);
        flag_t_set_flag(fl, INPUT);
        command_t_set_input_stream(input);

        if (output != NULL) {
          stdout_old = dup(STDOUT_FILENO);
          flag_t_set_flag(fl, OUTPUT);
          command_t_set_output_stream(output);
        }
      }

      if (commands[i]->background_process) {
        flag_t_set_flag(fl, BACKGROUND);
      }
    }

    if (pipe_line_position > 0) {
      /*
       * Invoke a series of commands in a pipe
       */
      run_pipeline(pipeline);
    } else {
      /*
       * Invoke solitary command
       */
      /*printf("[DEBUG] Running solo command\n");*/
      command_t_invoke(commands[0]);
    }

    /*
     * Reset output for both STDIN and STDOUT
     * SRC:
     * https://stackoverflow.com/questions/11042218/c-restore-stdout-to-terminal
     * These cannot be reset unless preset by the above logic
     */

    if (get_flag(fl, INPUT)) {
      restore_fd(stdin_old, STDIN_FILENO);
    }
    if (get_flag(fl, OUTPUT)) {
      restore_fd(stdout_old, STDOUT_FILENO);
    }

    clear_flags(fl);
  }

  /*
   * for some reason, invoking the destructor in the while/for loop
   * there is an overflow bug in the following iterations (after 1st)
   * but it seems to be fixed when doing a garbage collection at the very end
   * why this works, i have no idea. I truly don't.
   */

  harvester_of_sorrow(garbage, garbage_position, fl, history);

  return 0;
}

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * Executes the command "grep Villanova < scores > out".
 *
 * @author Jim Glenn
 * @version 0.1 9/23/2004
 *
 * @addtion author Jared Dyreson
 * thank you kind sir
 * PoC for shed
 */

int main(int argc, char **argv){
  int in = 0, out = 0;
  char *grep_args[] = {"grep", "Villanova", NULL};

  char* full_command[] = {
      "grep",
      "Villanova",
      "<",
      "scores",
      ">",
      "out",
      NULL
  };

  char* something[10];
  int j = 0;
  int max_ = 6;
  int stop_parsing = 0;
  for(int i = 0; i < max_; ++i){
      if(stop_parsing){
          printf("adding latest command into container\n");
          /*stop_parsing = 0;*/
      }

      if(strcmp(full_command[i], "<") == 0){
          // we need to use the contents of the next index as the file to use
          if(i+1 <= max_){
              printf("[INFO] Stop processing, use new command\n");
              printf("the input file would be %s\n", full_command[i+1]);
              in = open(full_command[i+1], O_RDONLY);
              if(in == EOF){
                  fprintf(stderr, "could not open %s for reading, cowardly refusing\n", full_command[i+1]);
              }
              stop_parsing = 1;
              // signal end of processing command_t and store it for future execution
          } else{
              fprintf(stderr, "parsing error, no input file to be found\n");
          }
      } else if(strcmp(full_command[i], ">") == 0){
          // we need to use the contents of the last command as the input for the out file
          if(i+1 <= max_){
              printf("the output file would be %s\n", full_command[i+1]);
              out = open(full_command[i+1], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
              // signal end of processing command_t and store it for future execution
          } else{
              fprintf(stderr, "parsing error, no output file to be found\n");
          }
      } else if(!stop_parsing) {
          something[j++] = strdup(full_command[i]);
            printf("adding %s to current command_t\n", full_command[i]);
      }
  }
  // these could be considered streams
  // in each of the command_t objects
  if(in){
      dup2(in, 0);
      close(in);
  } else if(out){
      dup2(out, 1);
      close(out);
  }

  for(int k = 0; k < j; ++k){
      printf("[%d] %s\n", k, something[k]);
  }
  something[++j] = NULL;

  /*char* string = ">";*/
  /*if(strcmp(string, ">") == 0){*/
      /*char* path = "out";*/
      /*out = open(path, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);*/
  /*} else if(strlen(string) == 0){*/
      /*out = EOF;*/
  /*} else if(strcmp(string, "<") == 0){*/
      /*char* path = "in";*/
      /*in = open(path, O_RDONLY);*/
  /*}*/

  /*if(out != EOF){*/
      /*dup2(out, 1);*/
  /*}*/
  /*dup2(in, 0);*/

  // open input and output files

  /*in = open("scores", O_RDONLY);*/
  /*out = open(stdout, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);*/

  // replace standard input with input file

  /*dup2(in, 0);*/

  // replace standard output with output file

  /*dup2(out, 1);*/

  // close unused file descriptors

  /*close(in);*/
  /*close(out);*/

  // execute grep

  execvp(something[0], something);
}

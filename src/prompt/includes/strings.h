#pragma once
#define MAX_LINE_SIZE 100
#define MAX_STR_COUNT 10
#define COM_SIZ 10

#define STDOUT_DIR 0
#define STDIN_DIR 1
#define PIPE_IN 2
#define BACKGROUND 3

char* get_line();
char* strip(char*, char);
struct command_t** parse_line(char*);

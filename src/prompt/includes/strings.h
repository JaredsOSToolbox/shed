#pragma once
#define MAX_LINE_SIZE 100

char* get_line();
char* strip(char*, char);
struct command_t** parse_line(char*);

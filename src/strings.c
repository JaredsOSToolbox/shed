#include "../includes/strings.h"
#include "../includes/command_t.h"
#include "../includes/flag_t.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char* get_line() {
    /*
     * We want to grab the line from stdin
     */
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    char* retline = (char*)malloc(MAX_LINE_SIZE * sizeof(char*));

    while ((read = getline(&line, &len, stdin)) != EOF) {
        if (line[0] == 0x0A) {
            free(retline);
            return NULL;
        }
        size_t s_len = strlen(line);
        if (line[s_len - 1] == 0x0A) {
            line[s_len - 1] = '\0';
            break;
        }
    }
    if (strlen(line) == 0) {
        // EOF
        free(retline);
        return "EOF";
    }

    strcpy(retline, line);
    return retline;
}

char* strip(char* string, char delimiter) {
    /*
     * Essentially a Python equivalent of str.strip()
     * It mangles the original string and returns a sanitized one
    */

    char* stripped = (char*)malloc(sizeof(char*));
    int j = 0;
    for (size_t i = 0; i < strlen(string); ++i) {
        if (string[i] != delimiter) {
            stripped[j++] = string[i];
        }
    }
    stripped[++j] = '\0';
    return stripped;
}

struct command_t** parse_line(char* input) {
    struct command_t** container = malloc(COM_SIZ * sizeof(struct command_t*));

    char** strings = malloc(MAX_STR_COUNT * sizeof(char*));
    struct flag_t* fl = flag_t_constructor();

    for (int p = 0; p < MAX_STR_COUNT; ++p) {
        strings[p] = (char*)malloc(BUFSIZ * sizeof(char));
    }

    char delimit[] = "|<>&";
    size_t delim_size = strlen(delimit);
    
    /*
     * i : indexing position of string that is being copied
     * j : position of command_t in command_t** buffer
     * n : nth string allocated, not exceeding MAX_STR_COUNT
     * t : counter to the number of chevrons (< | >) that occur
    */

    int i = 0, j = 0, n = 0;

    while (*input != '\0' && j <= COM_SIZ && n <= MAX_STR_COUNT) {
        for (size_t k = 0; k < delim_size; ++k) {
            if (*input == delimit[k]) {
                strings[n++][++i] = '\0';
                i = 0;
                struct command_t* command = command_t_constructor(strings[n-1]);
                switch (*input) {
                    case '|':
                        // signal to current command that it needs to look in
                        // the command_t* buffer as a look ahead here you could
                        // set the output stream?
                        flag_t_set_flag(fl, PIPE);
                        command->pipe_stream = 1;
                        break;
                    case '>':
                        flag_t_set_flag(fl, OUTPUT);
                        command->output_stream = 1;
                        break;
                    case '<':
                        flag_t_set_flag(fl, INPUT);
                        command->input_stream = 1;
                        break;
                    case '&':
                        flag_t_set_flag(fl, BACKGROUND);
                        command->background_process = 1;
                        break;
                }
                container[j++] = command;
                //
                while ((isspace(*input) || *input == delimit[k]) && (*input != '\0')) {
                    // advance string pointer further to trim off excess delimiters and whitespace
                    input++;
                }
            }
        }
        strings[n][i++] = *input++;
    }
    /*
     * Two cases we can have here:
     * 1) We have a dangling pipe and needs to be given to the current command
     * 2) Only one command was given and needs to be ran
    */

    strings[n][++i] = '\0';
    struct command_t* comm = command_t_constructor(strings[n]);

    if(*input == '\0' && get_flag(fl, PIPE)){
        comm->pipe_stream = true;
    }

    if(*input == '\0' && (get_flag(fl, OUTPUT) || get_flag(fl, INPUT))){
        if(strlen(strings[n]) == 0){
            comm->output_stream = 0;
        } else {
            comm->stream_path = strdup(strings[n]);
        }
    }
    if(*input == '\0' && get_flag(fl, BACKGROUND)){
        comm->background_process = true;
    }

    // no matches to the above delimiters, just regular command
    container[j++] = comm;

    container[j+1] = NULL;

    flag_t_destructor(fl);
    return container;
}


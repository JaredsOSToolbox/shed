#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "includes/command_t.h"
#include "includes/history.h"
#include "includes/strings.h"


int main(int argc, const char* argv[]) {

    struct command_t** commands = parse_line("echo line | grep -o l");
    int i = 0, z = 0;

    while(commands[z] != NULL) {
        if(commands[z]->pipe_stream && (z + 1 < COM_SIZ)){
            command_t_set_pipe_stream(commands[z])
        }
    }

    return 0;
}

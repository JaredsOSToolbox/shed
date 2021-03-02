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

    return 0;
}

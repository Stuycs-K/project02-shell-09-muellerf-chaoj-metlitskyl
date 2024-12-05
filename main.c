#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parse.h"

static void handle_interrupt(int signo) {
    if (signo == SIGINT) {
    }
}

int main(int argc, char **argv) {
    signal(SIGINT, handle_interrupt);
    char buffer[256];
    while (fgets(buffer, 255, stdin) != NULL) {
        handle_line_input(buffer);
    }
    return 0;
}

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parse.h"
#include "utils.h"

static void handle_interrupt(int signo) {
    if (signo == SIGINT) {
    }
}

int main(int argc, char **argv) {
    signal(SIGINT, handle_interrupt);
    print_prompt();
    char buffer[256];
    while (fgets(buffer, 255, stdin) != NULL) {
        handle_line_input(buffer);
        print_prompt();
    }
    printf("\nexit\n");
    return 0;
}

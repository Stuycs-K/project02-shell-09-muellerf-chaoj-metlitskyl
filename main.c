#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parse.h"
#include "utils.h"

/* 
 * Catches SIGINT (^C) signal, does nothing.
 * NOTE: Is a static function.
 * @param signo: signal number
 * @return: void
*/
static void handle_interrupt(int signo) {
    if (signo == SIGINT) {
    }
}

/*
 * // Main function. Takes in command line arguments & calls other functions
 * to check for SIGINT, print prompt, read input, and execute commands.
 * @param argc: number of command line arguments
 * @param argv: string array of command line arguments
 * @return: int = 0
*/
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

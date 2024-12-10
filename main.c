#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parse.h"
#include "utils.h"
#include "token.h"
#include "exec.h"

static void handle_interrupt(int signo) {
    if (signo == SIGINT) {
    }
}

int main(int argc, char **argv) {
    signal(SIGINT, handle_interrupt);
    print_prompt();
    char buffer[256];
    while (fgets(buffer, 255, stdin) != NULL) {
        struct token_list *tokens = tokenize(buffer);
        for (int i = 0; i < tokens->size; i++) {
            print_token(&tokens->tokens[i]);
        }
        //struct program *program = parse_program(tokens);
        //run(program);
        print_prompt();
    }
    printf("\nexit\n");
    return 0;
}

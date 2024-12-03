#include "parse.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void parse_args(char *line, char **arg_ary) {
    char *front = line;
    char *token;
    int c = 0;

    while ((token = strsep(&front, " ")) != NULL) {
        char *saved_token = calloc(strlen(token), sizeof(char));
        strcpy(saved_token, token);
        arg_ary[c] = saved_token;
        c++;
    }
}

void handle_line_input(char * buffer) {
    long arg_max = sysconf(_SC_ARG_MAX);
    buffer[strcspn(buffer, "\r\n")] = 0;
    if (strcmp(buffer, "exit") == 0) {
        exit(0);
    }
    char ** cmdargv = calloc(arg_max, sizeof(char *));
    parse_args(buffer, cmdargv);
    execvp(cmdargv[0], cmdargv);
}

#include "parse.h"
#include "string.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void handleChild(char **cmdargv) {
    execvp(cmdargv[0], cmdargv);
}

void handlePossibleForkFail(pid_t p) {
    if (p < 0) {
        char err[256];
        sprintf(err, "Error: %s\n", strerror(errno));
        perror(err); // output to stderr instead of stdout
        exit(1);
    }
}

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

void handle_line_input(char *buffer) {
    long arg_max = sysconf(_SC_ARG_MAX);
    buffer[strcspn(buffer, "\r\n")] = 0;
    if (strcmp(buffer, "exit") == 0) {
        exit(0);
    }
    char **cmdargv = calloc(arg_max, sizeof(char *));
    parse_args(buffer, cmdargv);

    pid_t child_one;
    child_one = fork();
    handlePossibleForkFail(child_one);
    if (child_one == 0) { // 0 if this is the child
        handleChild(cmdargv);
    } else {
        int status;
        waitpid(child_one, &status, 0); // if ran with htop and CONTROL+C(siqinterrupt) it breaks shell
        printf("Child %d finished!\n", child_one);
        // parent
    }
}

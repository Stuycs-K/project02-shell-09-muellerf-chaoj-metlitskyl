#include "parse.h"
#include "string.h"
#include "redirection.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void handleChild(char **cmdargv) { execvp(cmdargv[0], cmdargv); }

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
    // reset stdout and stdin first in here


    long arg_max = sysconf(_SC_ARG_MAX);
    buffer[strcspn(buffer, "\r\n")] = 0;
    if (strcmp(buffer, "exit") == 0) {
        exit(0);
    }
    // find all stuff after '>'
    int MODE = 0; // 0: do nothing mode, 1: write, 2: append, 3: input
    char *str_out;
    str_out = strchr(buffer, '>');
    if (str_out != NULL) {
        str_out++; /* step over the greater than */
        if (str_out[0] == '>'){ // second '>' means append mode
            MODE = 2;
            str_out += 2; // step over the '>' and space
        } else {
            MODE = 1;
            str_out += 1; // step over the space
        }
        // if str_out specified, end buffer early;
        buffer[strlen(buffer) - strlen(str_out) - MODE - 2] = '\0'; // MODE is a proxy for chars removed too :)
        printf("|%s|\n",buffer);
        printf("Stuff after >:%s\n", str_out);
        printf("Mode:%d\n", MODE);
    }
    char *str_in;
    str_in = strchr(buffer, '<');
    if (str_in != NULL) {
        str_in++; /* step over the less than */
        str_in++; /* step over the space */
        MODE = 3;
        // if str_in specified, end buffer early;
        buffer[strlen(buffer) - strlen(str_in) - 1 - 2] = '\0';
        printf("|%s|\n", buffer);
        printf("Stuff after <:%s\n", str_in);
        printf("(input) Mode:%d\n", MODE);
    }
    
    int backup_stdout = dup(STDOUT_FILENO);
    int backup_stdin = dup(STDIN_FILENO);
    if (MODE == 1){
        stdout_redirect(str_out);
    } else if (MODE == 2){
        stdout_redirect_append(str_out);
    } else if (MODE == 3){
        stdin_redirect(str_in); // DOES NOT HANDLE BOTH < and > used at once
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
        waitpid(child_one, &status, 0);
        // revert file table
        dup2(backup_stdout, STDOUT_FILENO );
        dup2(backup_stdin,STDIN_FILENO );
    }
}

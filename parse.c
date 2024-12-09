#include "parse.h"
#include "string.h"
#include "redirection.h"
#include "utils.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void handleChild(char **cmdargv) {
    execvp(cmdargv[0], cmdargv);
    if (errno == 2) { // command not found
        printf("%s: command not found...\n", cmdargv[0]);
    } else {
        perror(strerror(errno));
    }
    exit(errno); // this is only executed if execvp fails (so child does not keep on living)
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

int parse_commands_of_pipe(char *line, char **command_ary) {
    char *front = line;
    char *token;
    int c = 0;

    while ((token = strsep(&front, "|")) != NULL) {
        char *saved_token = calloc(strlen(token), sizeof(char));
        strcpy(saved_token, token);
        command_ary[c] = saved_token;
        c++;
    }
    return c;
}

void handle_line_input(char *buffer) {
    // i bet no kid thought of this temp file name (yes i know this can just be dynamic but we are lazy :) )
    char *TEMP_FILE = "/tmp/pipevjZQ3SZpv2UygTLp4RWE.txt"; // string LITERAL can't be modified

    int backup_stdout = dup(STDOUT_FILENO);
    int backup_stdin = dup(STDIN_FILENO);

    long arg_max = sysconf(_SC_ARG_MAX);
    buffer[strcspn(buffer, "\r\n")] = 0;
    if (strcmp(buffer, "exit") == 0) {
        exit(0);
    }

    // split on all instances of pipes
    
    char **lineargv = calloc(1024, sizeof(char *));
    int commands_len = parse_commands_of_pipe(buffer, lineargv);

    if (commands_len > 1){ // if more than one command
        for (int i = 0; i < commands_len; i++){
            if (lineargv[i][0] == ' '){
                lineargv[i]++; // shift left
            }
            if (lineargv[i][strlen(lineargv[i]) - 1] == ' '){
                lineargv[i][strlen(lineargv[i]) - 1] = '\0'; // shift right
            }

            if (i == 0){ // if first command
                stdout_redirect(TEMP_FILE);   // overwrites and truncates temp file
            } else if (i == 1){
                stdin_redirect(TEMP_FILE);   
            }

            handle_line_input(lineargv[i]);

            // revert file table
            dup2(backup_stdout, STDOUT_FILENO);
            dup2(backup_stdin, STDIN_FILENO);
        }
        
        return; // handled all sub pipes already 
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
        // printf("|%s|\n",buffer);
        // printf("Stuff after >:%s\n", str_out);
        // printf("Mode:%d\n", MODE);
    }
    char *str_in;
    str_in = strchr(buffer, '<');
    if (str_in != NULL) {
        str_in++; /* step over the less than */
        str_in++; /* step over the space */
        MODE = 3;
        // if str_in specified, end buffer early;
        buffer[strlen(buffer) - strlen(str_in) - 1 - 2] = '\0';
        // printf("|%s|\n", buffer);
        // printf("Stuff after <:%s\n", str_in);
        // printf("(input) Mode:%d\n", MODE);
    }
    
    if (MODE == 1){
        stdout_redirect(str_out);
    } else if (MODE == 2){
        stdout_redirect_append(str_out);
    } else if (MODE == 3){
        stdin_redirect(str_in); // DOES NOT HANDLE BOTH < and > used at once
    }

    

    char **cmdargv = calloc(arg_max, sizeof(char *));
    parse_args(buffer, cmdargv);
    if (strcmp(cmdargv[0], "") == 0 || cmdargv[0][0] == 27) {
        return; // don't do anything if blank or escape char (execing nothing just forks unncessarily)
    }

    if (strcmp(cmdargv[0], "cd") == 0) {
        char *cd_path = cmdargv[1];
        if (cmdargv[1] == NULL) { // no argument was passed to cd
            cd_path = get_home_dir();
        }
        int cd_return_value = chdir(cd_path);
        if (cd_return_value != 0) {
            perror(strerror(errno));
        }
        return;
    }

    pid_t child_one;
    child_one = fork();
    handlePossibleForkFail(child_one);
    if (child_one == 0) { // 0 if this is the child
        handleChild(cmdargv);
    } else {
        int status;
        waitpid(child_one, &status, 0);
        // revert file table
        dup2(backup_stdout, STDOUT_FILENO);
        dup2(backup_stdin, STDIN_FILENO);
    }
}

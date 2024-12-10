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

/*
 * Executes given user command arguments using execvp.
 * Exits with error message if command not found.
 * NOTE: Only used in this file.
 * @param cmdargv: string array of command arguments
 * @return: void
*/
void handleChild(char **cmdargv) {
    execvp(cmdargv[0], cmdargv);
    if (errno == 2) { // command not found
        printf("%s: command not found...\n", cmdargv[0]);
    } else {
        perror(strerror(errno));
    }
    exit(errno); // this is only executed if execvp fails (so child does not keep on living)
}

/*
 * Checks if fork failed. Prints error message and exits if true.
 * NOTE: Only used in this file.
 * @param p: pid
 * @return: void
*/
void handlePossibleForkFail(pid_t p) {
    if (p < 0) {
        char err[256];
        sprintf(err, "Error: %s\n", strerror(errno));
        perror(err); // output to stderr instead of stdout
        exit(1);
    }
}

/*
 * Splits string line on given character separator and loads parts into string array.
 * NOTE: Only used in this file.
 * NOTE: Even though split is a string, it should only be one character.
 * @param line: string to split
 * @param split: character to split on
 * @param command_ary: empty string array to load split parts into
 * @return: int = number of commands
*/
int split_on_string(char *line, char *split, char **command_ary) {
    char *front = line;
    char *token;
    int c = 0;

    while ((token = strsep(&front, split)) != NULL) {
        char *saved_token = calloc(strlen(token), sizeof(char));
        strcpy(saved_token, token);
        command_ary[c] = saved_token;
        c++;
    }
    return c;
}

/*
 * Executes main bulk of shell program. Takes in buffer, checks for exit, else 
 * splits buffer on semicolons and redirections. Executes commands in child processes.
 * @param buffer: input string buffer
 * @return: void
*/
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

    if (buffer[0] == ' '){ // one char left strip
        buffer++; // shift left
    }
    if (buffer[strlen(buffer) - 1] == ' '){ // one char right strip
        buffer[strlen(buffer) - 1] = '\0'; // shift right
    }

    // split on all instances of semicolons FIRST
    char **semi_colon_lines_argv = calloc(4096, sizeof(char *));
    int semi_colon_commands_len = split_on_string(buffer, ";", semi_colon_lines_argv); // also loads into semi_colon_lines_argv

    if (semi_colon_commands_len > 1){ // if more than one command
        for (int i = 0; i < semi_colon_commands_len; i++){
            handle_line_input(semi_colon_lines_argv[i]);

            // revert file table
            dup2(backup_stdout, STDOUT_FILENO);
            dup2(backup_stdin, STDIN_FILENO);
        }
        
        return; // handled all sub semi colons already 
    }
    
    // split on all instances of pipes
    char **lineargv = calloc(1024, sizeof(char *));
    int pipe_commands_len = split_on_string(buffer, "|", lineargv);

    if (pipe_commands_len > 1){ // if more than one command
        for (int i = 0; i < pipe_commands_len; i++){
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
    }
    char *str_in;
    str_in = strchr(buffer, '<');
    if (str_in != NULL) {
        str_in++; /* step over the less than */
        str_in++; /* step over the space */
        MODE = 3;
        // if str_in specified, end buffer early;
        buffer[strlen(buffer) - strlen(str_in) - 1 - 2] = '\0';
    }
    
    if (MODE == 1){
        stdout_redirect(str_out);
    } else if (MODE == 2){
        stdout_redirect_append(str_out);
    } else if (MODE == 3){
        stdin_redirect(str_in); // DOES NOT HANDLE BOTH < and > used at once
    }


    char **cmdargv = calloc(arg_max, sizeof(char *));
    split_on_string(buffer, " ", cmdargv); // this returns number of cmds (ignored) and ALSO loads in cmdargv
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

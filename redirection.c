#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

/*
 * Redirects stdout to output file (OVERWRITE).
 * NOTE: Flushes stdout upon completion.
 * @param output: output file name
 * @return: void
*/
void stdout_redirect(char * output) { 
    int output_file = open(output, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    dup2(output_file, STDOUT_FILENO);
    close(output_file);
    fflush(stdout);
}

/*
 * Redirects stdout to output file (APPEND).
 * NOTE: Flushes stdout upon completion.
 * @param output: output file name
 * @return: void
*/
void stdout_redirect_append(char * output) {
    int output_file = open(output, O_CREAT | O_WRONLY | O_APPEND, 0644);
    dup2(output_file, STDOUT_FILENO);
    close(output_file);
    fflush(stdout);
}

/*
 * Redirects input file to stdin.
 * NOTE: Flushes stdin upon completion.
 * @param input: input file name
 * @return: void
*/
void stdin_redirect(char * input) {
    int input_file = open(input, O_RDONLY);
    dup2(input_file, STDIN_FILENO);
    fflush(stdin);
}

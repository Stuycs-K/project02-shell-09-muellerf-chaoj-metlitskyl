#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

void stdout_redirect(char * output) { // Takes in output file name. Redirects stdout to output file (OVERWRITE).
    int output_file = open(output, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    dup2(output_file, STDOUT_FILENO);
    close(output_file);
    fflush(stdout);
}

void stdout_redirect_append(char * output) { // Takes in output file name. Redirects stdout to output file (APPEND).
    int output_file = open(output, O_CREAT | O_WRONLY | O_APPEND, 0644);
    dup2(output_file, STDOUT_FILENO);
    close(output_file);
    fflush(stdout);
}

void stdin_redirect(char * input) { // Takes in input file name. Redirects input file to stdin for program.
    int input_file = open(input, O_RDONLY);
    dup2(input_file, STDIN_FILENO);
    fflush(stdin);
}

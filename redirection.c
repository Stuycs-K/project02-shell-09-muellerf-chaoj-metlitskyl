#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

void stdout_redirect(char * output) {
    int output_file = open(output, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    dup2(output_file, STDOUT_FILENO);
    close(output_file);
    fflush(stdout);
}

void stdout_redirect_append(char * output) {
    int output_file = open(output, O_CREAT | O_WRONLY | O_APPEND, 0644);
    dup2(output_file, STDOUT_FILENO);
    close(output_file);
    fflush(stdout);
}

void stdin_redirect(char * input) {
    int input_file = open(input, O_RDONLY);
    // int backup = dup(STDIN_FILENO);
    dup2(STDIN_FILENO, input_file);
    printf("File redirected into stdin.");
    fflush(stdin);
    // dup2(stdin, backup);
}

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

void stdout_redirect(char * output) {
    int output_file = open(output, O_WRONLY | O_TRUNC, 0644);
    int backup = dup(STDOUT_FILENO);
    dup2(output_file, STDOUT_FILENO);
    printf("stdout redirected.");
    close(output_file);
    fflush(stdout);
    // dup2(stdout, backup);
}

void stdout_redirect_append(char * input, char * output) {
    int output_file = open(output, O_WRONLY | O_APPEND, 0644);
    int backup = dup(STDOUT_FILENO);
    dup2(output_file, STDOUT_FILENO);
    printf("stdout redirected.");
    close(output_file);
    fflush(stdout);
    // dup2(stdout, backup);
}

void stdin_redirect(char * input) {
    int backup = dup(STDIN_FILENO);
    dup2(STDIN_FILENO, input);
    printf("File redirected into stdin.");
    fflush(stdin);
    // dup2(stdin, backup);
}

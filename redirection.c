#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

void stdout_redirect(char * output) {
    int output_file = open(output, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int stdout = STDOUT_FILENO;
    int backup = dup(stdout);
    dup2(output_file, stdout);
    printf("File successfully redirected.");
    close(output_file);
    fflush(stdout); // not necessary in child processes
    dup2(backup, stdout);
}

void stdout_redirect_append(char * input, char * output) {
    int output_file = open(output, O_WRONLY | O_CREAT | O_APPEND, 0644);
    int stdout = STDOUT_FILENO;
    int backup = dup(stdout);
    dup2(output_file, stdout);
    printf("File successfully redirected.");
    close(output_file);
    fflush(stdout);
    dup2(backup, stdout);
}

void stdin_redirect(char * input) {
    // may not be necessary as you can just use the new file as stdin once '<' is detected
}

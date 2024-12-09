#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

int stdout_redirect_random() {
    char template[] = "pipeXXXXXX";
    int output_file = mkstemp(template);
    dup2(output_file, STDOUT_FILENO);
    close(output_file);
    fflush(stdout);
    return output_file;
}
void stdin_redirect_random(int input) {
    dup2(input, STDIN_FILENO);
    fflush(stdin);
}

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
    dup2(input_file, STDIN_FILENO);
    fflush(stdin);
}

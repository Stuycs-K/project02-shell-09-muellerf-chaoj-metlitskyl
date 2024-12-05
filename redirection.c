#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void stdout_redirect(char * output) {
    int output_file = open(output, 0_WRONLY | O_TRUNC | O_CREAT);
    int stdout = STDOUT_FILENO;
    dup2(output_file, stdout);
    printf("File successfully redirected.");
}

void stdout_redirect_append(char * input, char * output) {
    
}

void stdin_redirect() {
    
}

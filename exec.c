#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

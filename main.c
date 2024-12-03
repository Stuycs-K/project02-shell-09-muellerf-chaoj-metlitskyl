#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
    char buffer[256];
    long arg_max = sysconf(_SC_ARG_MAX);
    while (fgets(buffer, 255, stdin) != NULL) {
        buffer[strcspn(buffer, "\r\n")] = 0;
        if (strcmp(buffer, "exit") == 0) {
            exit(0);
        }
        char ** cmdargv = calloc(arg_max, sizeof(char *));
        parse_args(buffer, cmdargv);
        execvp(cmdargv[0], cmdargv);
    }
    return 0;
}

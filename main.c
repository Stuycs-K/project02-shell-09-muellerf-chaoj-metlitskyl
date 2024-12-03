#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parse.h"

int main(int argc, char **argv) {
    char buffer[256];
    while (fgets(buffer, 255, stdin) != NULL) {
        handle_line_input(buffer);
    }
    return 0;
}

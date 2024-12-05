#include "utils.h"
#include <stdio.h>
#include <unistd.h>
#include <linux/limits.h>

void print_prompt() {
    char buf[PATH_MAX];
    getcwd(buf, PATH_MAX);
    printf("%s$ ", buf);
    fflush(stdout);
}

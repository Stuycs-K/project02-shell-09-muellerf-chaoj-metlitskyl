#include "utils.h"
#include <bits/local_lim.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void print_prompt() {
    char username[LOGIN_NAME_MAX + 1];
    getlogin_r(username, sizeof(username)); // check the return value!

    char hostname[HOST_NAME_MAX + 1];
    gethostname(hostname, sizeof(hostname)); // check the return value!

    char cwd[PATH_MAX];
    getcwd(cwd, PATH_MAX); // check the return value!

    char prompt[LOGIN_NAME_MAX + HOST_NAME_MAX + PATH_MAX + 512] = "";
    sprintf(prompt, "%s@%s %s$ ", username, hostname, cwd);

    // replace "/home/user" from cwd
    char home_prefix[] = "/home/";
    size_t path_target_len = strlen(home_prefix) + strlen(username) + 1;
    char *path_target = calloc(path_target_len, sizeof(char));
    strcat(path_target, home_prefix);
    strcat(path_target, username);

    printf(prompt);
    fflush(stdout);
}

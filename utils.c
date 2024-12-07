#include "utils.h"
#include <bits/local_lim.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int prefix(const char *pre, const char *str) {
    return strncmp(pre, str, strlen(pre)) == 0;
}

void print_prompt() {
    char username[LOGIN_NAME_MAX + 1];
    getlogin_r(username, sizeof(username)); // check the return value!

    char hostname[HOST_NAME_MAX + 1];
    gethostname(hostname, sizeof(hostname)); // check the return value!

    char *cwd = calloc(PATH_MAX, sizeof(char));
    char *cwd_formatted = cwd;
    getcwd(cwd, PATH_MAX); // check the return value!
                           // replace "/home/user" from cwd
    char home_prefix[] = "/home/";
    size_t path_target_len = strlen(home_prefix) + strlen(username) + 1;
    char *path_target = calloc(path_target_len, sizeof(char));
    strcat(path_target, home_prefix);
    strcat(path_target, username);
    if (prefix(path_target, cwd)) {
        cwd_formatted[path_target_len - 2] = '~';
        cwd_formatted = cwd + path_target_len - 2; // subtract 2 instead of 1 to account for '~'
    }

    char prompt[LOGIN_NAME_MAX + HOST_NAME_MAX + PATH_MAX + 512] = "";
    sprintf(prompt, "%s@%s %s$ ", username, hostname, cwd_formatted);

    printf(prompt);
    fflush(stdout);
    free(cwd);
}

#include "utils.h"
#include <bits/local_lim.h>
#include <linux/limits.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

/*
 * Compares two strings to see if the first is a prefix of the second.
 * NOTE: Only used in this file.
 * @param pre: prefix string
 * @param str: string to check for prefix
 * @return: int = 1 if prefix exists, 0 otherwise
*/
int prefix_cmp(const char *pre, const char *str) { 
    return strncmp(pre, str, strlen(pre)) == 0;
}

/*
 * Prints prompt (with username, hostname, and ~ substituted for home dir) to stdout.
 * NOTE: ~ may not appear in Windows because of different home directory structure from Linux.
 * @param: void
 * @return: void
*/
void print_prompt() {
    char username[LOGIN_NAME_MAX + 1];
    getlogin_r(username, sizeof(username)); // check the return value!

    char hostname[HOST_NAME_MAX + 1];
    gethostname(hostname, sizeof(hostname)); // check the return value!

    char *cwd = calloc(PATH_MAX, sizeof(char));
    char *cwd_formatted = cwd;
    getcwd(cwd, PATH_MAX); // check the return value!
                           // replace "/home/user" from cwd
    char *home_dir = get_home_dir();
    if (prefix_cmp(home_dir, cwd)) {
        cwd_formatted[strlen(home_dir) - 1] = '~';
        cwd_formatted = cwd + strlen(home_dir) - 1; // subtract 2 instead of 1 to account for '~'
    }

    char prompt[LOGIN_NAME_MAX + HOST_NAME_MAX + PATH_MAX + 512] = "";
    sprintf(prompt, "%s@%s %s$ ", username, hostname, cwd_formatted);

    printf("%s", prompt);
    fflush(stdout);
    free(cwd);
}

/*
 * Gets home directory of current user.
 * NOTE: Only used in this file.
 * @param: void
 * @return: char * = home directory of current user
*/
char *get_home_dir() {
    struct passwd *pw = getpwuid(getuid());
    char *homedir = pw->pw_dir;
    // pointer does NOT need to be freed here (free doesn't work on it even)
    // so no need to strcpy if the memory is something special here
    return homedir;
}
[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Tfg6waJb)
# Systems Project2 - MyShell
#### Group Members
 - Flint Mueller
 - Lenny Metlitsky
 - Jason Chao

 ## Features
 - Successful execution of commands.
    - Spaces to separate command arguments.
    - Use semicolon to separate multiple commands.
    - Redirection using ```>```, ```>>```, ```<```, and one pipe (using a temp file).
 - Prompt displays:
    - ```username@hostname``` before current working directory.
    - Replaces ```~``` with user home directory.

## Unsuccessful
 - Unable to execute commands with multiple pipes.

## Bugs
We have not found any bugs in our shell.

## Function Headers
```main.c```
```
/* 
 * Catches SIGINT (^C) signal, does nothing.
 * NOTE: Is a static function.
 * @param signo: signal number
 * @return: void
*/
static void handle_interrupt(int signo);

/*
 * // Main. Takes in command line arguments & calls other functions
 * to check for SIGINT, print prompt, read input, and execute commands.
 * @param argc: number of command line arguments
 * @param argv: string array of command line arguments
 * @return: int = 0
*/
int main(int argc, char **argv);
```

```utils.c```
```
/*
 * Compares two strings to see if the first is a prefix of the second.
 * NOTE: Only used in this file.
 * @param pre: prefix string
 * @param str: string to check for prefix
 * @return: int = 1 if prefix exists, 0 otherwise
*/
int prefix_cmp(const char *pre, const char *str);

/*
 * Prints prompt (with username, hostname, and ~ substituted for home dir) to stdout.
 * NOTE: ~ may not appear in Windows because of different home directory structure from Linux.
 * @param: void
 * @return: void
*/
void print_prompt();

/*
 * Gets home directory of current user.
 * NOTE: Only used in this file.
 * @param: void
 * @return: char * = home directory of current user
*/
char *get_home_dir();
```

```parse.c```
```
/*
 * Executes given user command arguments using execvp.
 * Exits with error message if command not found.
 * NOTE: Only used in this file.
 * @param cmdargv: string array of command arguments
 * @return: void
*/
void handleChild(char **cmdargv);

/*
 * Checks if fork failed. Prints error message and exits if true.
 * NOTE: Only used in this file.
 * @param p: pid
 * @return: void
*/
void handlePossibleForkFail(pid_t p);

/*
 * Splits string line on given character separator and loads parts into string array.
 * NOTE: Only used in this file.
 * NOTE: Even though split is a string, it should only be one character.
 * @param line: string to split
 * @param split: character to split on
 * @param command_ary: empty string array to load split parts into
 * @return: int = number of commands
*/
int split_on_string(char *line, char *split, char **command_ary);

/*
 * Executes main bulk of shell program. Takes in buffer, checks for exit, else 
 * splits buffer on semicolons and redirections. Executes commands in child processes.
 * @param buffer: input string buffer
 * @return: void
*/
void handle_line_input(char *buffer);
```

```redirection.c```
```
/*
 * Redirects stdout to output file (OVERWRITE).
 * NOTE: Flushes stdout upon completion.
 * @param output: output file name
 * @return: void
*/
void stdout_redirect(char * output);

/*
 * Redirects stdout to output file (APPEND).
 * NOTE: Flushes stdout upon completion.
 * @param output: output file name
 * @return: void
*/
void stdout_redirect_append(char * output);

/*
 * Redirects input file to stdin.
 * NOTE: Flushes stdin upon completion.
 * @param input: input file name
 * @return: void
*/
void stdin_redirect(char * input);
```
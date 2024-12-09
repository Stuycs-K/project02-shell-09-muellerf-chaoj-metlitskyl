[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Tfg6waJb)
# Systems Project2 - MyShell
## Group Members
 - Flint Mueller
 - Lenny Metlitsky
 - Jason Chao

 ### We don't have a creative team name (YET).

 ## Description
 Our shell implements Bash with the exception of several features like command history. However, it successfully runs commands with execvp and redirection with one pipe, and the shell prompt shows the username, host name, and path with ~/ substitution. We were unable to properly execute commands with several pipes, even though that is out of the scope for this project, and are currently fixing that on a separate branch.

 ## Function Headers
 - main.c: static void handle_interrupt(int signo); int main(int argc, char **argv);
 - parse.c: int split_on_string(char *line, char *split, char **command_ary); void handle_line_input(char * buffer);
 - utils.c: void print_prompt(); int prefix_cmp(const char *pre, const char *str); char *get_home_dir();
 - redirection.c: void stdout_redirect(char * output); void stdout_redirect_append(char * output); void stdin_redirect(char * input);
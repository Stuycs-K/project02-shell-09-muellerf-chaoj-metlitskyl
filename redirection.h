#ifndef REDIRECTION_H
#define REDIRECTION_H
int stdout_redirect_random();
void stdin_redirect_random(int input);
void stdout_redirect(char * output);
void stdout_redirect_append(char * output);
void stdin_redirect(char * input);
#endif

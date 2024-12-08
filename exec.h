#include <stdlib.h>

#ifndef EXEC_H
#define EXEC_H
void handleChild(char **cmdargv);
void handlePossibleForkFail(pid_t p);
#endif

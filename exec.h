#include <stdlib.h>
#include "parse.h"

#ifndef EXEC_H
#define EXEC_H
void run(struct program *program);
void handleChild(char **cmdargv);
void handlePossibleForkFail(pid_t p);
#endif

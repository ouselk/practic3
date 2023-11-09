#ifndef STACK_H
#define STACK_H

#define STACK_OVERFLOW  -1
#define STACK_UNDERFLOW -2
#define OUT_OF_MEMORY   -3

#include "node.h"

void stackPush(Node** head, char* data);
char* stackPop(Node** head);

#endif
#ifndef SET_H
#define SET_H

#include <stdbool.h>
#include "hashmap.h"

#define SET_CAPACITY 100

typedef HashElem Set;

void setAdd(Set** set, char* elem);
void setRem(Set** set, char* elem);
bool setIsMember(Set** set, char* elem);

#endif
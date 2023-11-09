#include <stdlib.h>
#include <stdio.h>
#include "set.h"


void setAdd(Set** set, char *elem)
{
    return hashmapAdd(set, elem, "\0");
}

void setRem(Set** set, char *elem)
{
    return hashmapDelete(set, elem);
}

bool setIsMember(Set** set, char *elem)
{
    char* result = hashmapGetValue(set, elem);
    if (result == NULL)
        return false;

    return true;
}
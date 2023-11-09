#ifndef HASHMAP_H
#define HASHMAP_H

#include "node.h"

#define HASHMAP_CAPACITY 100

typedef struct HashElem
{
    char* key;
    char* value;
    struct HashElem* overflow_bucket;
} HashElem;

int hashmapHash(const char* key);
void hashmapAdd(HashElem** map, char* key, char* value);
char* hashmapGetValue(HashElem** map, char* key);
void hashmapDelete(HashElem** map, char* key);

#endif
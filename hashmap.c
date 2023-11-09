#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashmap.h"

int hashmapHash(const char* key)
{
    int hashed = 0;
    while(*key!='\0')
    {
        hashed+= *key;
        key++;
    }

    return (hashed % HASHMAP_CAPACITY);
}

void hashmapAdd(HashElem** map, char* key, char* value)
{
    int index = hashmapHash(key);
    HashElem *newElem = (HashElem*)malloc(sizeof(HashElem));
    newElem->key = key;
    newElem->value = value;
    newElem->overflow_bucket = NULL;

    if (map[index] == NULL)
        {
            map[index] = newElem;
        }
    else
    {
        HashElem* tmp = map[index];
        while (tmp)
        {
            if (strcmp(tmp->key, key) == 0)
            {
                tmp->value = newElem->value;
                return;
            }
            tmp = tmp->overflow_bucket;
        }
        
        {
            newElem->overflow_bucket = map[index];
            map[index] = newElem;
        }
    }
}

char* hashmapGetValue(HashElem** map, char* key)
{
    int index = hashmapHash(key);
    HashElem* elem = map[index];
    
    
    while (elem!=NULL)
    {
        if (strcmp(elem->key, key) == 0)
            return elem->value;
        elem = elem->overflow_bucket;

    }

    return NULL;
}

void hashmapDelete(HashElem** map, char* key)
{
    int index = hashmapHash(key);
    HashElem* currentElem = map[index];
    HashElem* prevElem = NULL;
    while (currentElem!=NULL)
    {
        if (strcmp(currentElem->key, key)==0)
        {
            if (prevElem != NULL)
            {
                prevElem->overflow_bucket = currentElem->overflow_bucket;
            }
            else
            {
                map[index] = currentElem->overflow_bucket;
            }
         //   free(currentElem->key);
            free(currentElem);
            return;
        }
        prevElem = currentElem;
        currentElem = currentElem->overflow_bucket;
    }
}
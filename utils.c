#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stack.h"
#include "set.h"
#include "queue.h"
#include "hashmap.h"

Node* stackFromString(char* string)
{
    char* stringStack = string;

    char* buff = (char*)malloc(sizeof(char) * strlen(stringStack));
    Node* stack = NULL;

    while (*stringStack!='\0')
    {
        for (int i = 0; i<strlen(stringStack); i++)
            buff[i] = '\0';

        int i = 0;
        while (*stringStack!=';' && *stringStack!='\0')
        {
            buff[i++] = *stringStack;
            stringStack++;
        }
        stringStack++;
        char* value = strdup(buff);
        stackPush(&stack, value);
    }

    //Надо перевенуть стэк, потому что так надо
    Node* reverse = NULL;
    while (stack)
        stackPush(&reverse, stackPop(&stack));   

    free(buff);
    return reverse;
}

char* stackInString(Node* stack, char* name)
{
    if (!stack)
    {
        return "";
    }
    int stackstringlen = strlen(name)+3;
    Node* tmp = stack;
    while (tmp)
    {
        stackstringlen += strlen(tmp->data) + 1;
        tmp = tmp->next;
    }
    char* stackstring = (char*)malloc(sizeof(char) * stackstringlen );
    for (int i = 0; i<stackstringlen; i++)
        stackstring[i] = '\0';
    stackstring[0] = '\0';
    strcat(stackstring, "s");
    strcat(stackstring, " ");
    strcat(stackstring, name);
    strcat(stackstring, " ");
    while (stack)
    {
        char* value = stackPop(&stack);
        strcat(stackstring, value);
        strcat(stackstring, ";");
    }
    
    return stackstring;
}

Set** setFromString(char* string)
{
    char* stringStack = string;

    char* buff = (char*)malloc(sizeof(char) * strlen(stringStack));
    Set** set = malloc(sizeof(Set*) * SET_CAPACITY);
    for (int i = 0; i<SET_CAPACITY; i++)
        set[i] = NULL;

    while (*stringStack!='\0')
    {
        for (int i = 0; i<strlen(stringStack); i++)
            buff[i] = '\0';

        int i = 0;
        while (*stringStack!=';' && *stringStack!='\0')
        {
            buff[i++] = *stringStack;
            stringStack++;
        }
        stringStack++;
        char* value = strdup(buff);
        setAdd(set, value);
    }

    free(buff);
    return set;
}

char* setInString(Set** set, char* name)
{
    int stackstringlen = strlen(name)+3;

    for (int i = 0; i<SET_CAPACITY; i++)
    {
        Set* elem = set[i];
        while (elem)
        {
            stackstringlen+=strlen(elem->key) + 1;
            elem = elem->overflow_bucket;
        }
    }
    char* stackstring = (char*)malloc(sizeof(char) * stackstringlen );
    for (int i = 0; i<stackstringlen; i++)
        stackstring[i] = '\0';
    stackstring[0] = '\0';
    strcat(stackstring, "S");
    strcat(stackstring, " ");
    strcat(stackstring, name);
    strcat(stackstring, " ");
    for (int i = 0; i<SET_CAPACITY; i++)
    {
        Set* elem = set[i];
        while (elem)
        {
            strcat(stackstring, elem->key);
            strcat(stackstring, ";");
            elem = elem->overflow_bucket;
        }
    }

    return stackstring;
}

Queue* queueFromString(char* string)
{
    char* stringqueue = string;

    char* buff = (char*)malloc(sizeof(char) * strlen(stringqueue));
    Queue* queue = malloc(sizeof(Queue*));
    queue->head = NULL;
    queue->tail = NULL;

    while (*stringqueue!='\0')
    {
        for (int i = 0; i<strlen(stringqueue); i++)
            buff[i] = '\0';

        int i = 0;
        while (*stringqueue!=';' && *stringqueue!='\0')
        {
            buff[i++] = *stringqueue;
            stringqueue++;
        }
        stringqueue++;
        char* value = strdup(buff);
        queuePush(queue, value);
    }

    //Надо перевенуть очередь, потому что так надо
    Queue* reverse = malloc(sizeof(Queue*));
    reverse->head = NULL;
    reverse->tail = NULL;
    while (queue->head)
    {
        queuePush(reverse, queuePop(queue));  
    }



    free(buff);
    return reverse;
}

char* queueInString(Queue* queue, char* name)
{
    if (!queue || !(queue->head))
    {
        return "";
    }
    int queuestringlen = strlen(name)+3;
    Node* tmp = queue->head;
    while (tmp)
    {
        queuestringlen += strlen(tmp->data) + 1;
        tmp = tmp->next;
    }
    char* queuestring = (char*)malloc(sizeof(char) * queuestringlen );
    for (int i = 0; i<queuestringlen; i++)
        queuestring[i] = '\0';
    queuestring[0] = '\0';
    strcat(queuestring, "q");
    strcat(queuestring, " ");
    strcat(queuestring, name);
    strcat(queuestring, " ");
    while (queue->head)
    {
        char* value = queuePop(queue);
        strcat(queuestring, value);
        strcat(queuestring, ";");
    }
    
    return queuestring;
}

HashElem** hashmapFromString(char* string)
{
    char* stringhashmap = string;

    char* buff = (char*)malloc(sizeof(char) * strlen(stringhashmap));
    HashElem** hashmap = malloc(sizeof(HashElem*) * HASHMAP_CAPACITY);
    for (int i = 0; i < HASHMAP_CAPACITY; i++)
        hashmap[i] = NULL;

    if (string == NULL || strlen(string)<4)
        return hashmap;
 
    while (*stringhashmap!='\0')
    {
        for (int i = 0; i<strlen(stringhashmap); i++)
            buff[i] = '\0';

        int i = 0;
        while (*stringhashmap!=';' && *stringhashmap!='\0')
        {
            buff[i++] = *stringhashmap;
            stringhashmap++;
        }
        stringhashmap++;
        char* key = strdup(strtok(buff, ":"));
        char* value = strdup(strtok(NULL, ":"));
        hashmapAdd(hashmap, key, value);
    }


    free(buff);
    return hashmap;
}

char* hashmapInString(HashElem** hashmap, char* name)
{
    {
        bool empty = true;
        for (int i = 0; i < HASHMAP_CAPACITY; i++)
            if (hashmap[i])
            {
                empty = false;
                break;
            }
        if (empty)
            return "";
    }
    int hashmapstringlen = strlen(name)+3;
    
    for (int i = 0; i < HASHMAP_CAPACITY; i++)
    {
        HashElem* elem = hashmap[i];
        while (elem)
        {
            hashmapstringlen += strlen(elem->key) + 1;
            hashmapstringlen += strlen(elem->value) + 1;
            elem = elem->overflow_bucket;
        }
    }
    char* hashmapstring = (char*)malloc(sizeof(char) * hashmapstringlen );
    for (int i = 0; i<hashmapstringlen; i++)
        hashmapstring[i] = '\0';
    hashmapstring[0] = '\0';
    strcat(hashmapstring, "h");
    strcat(hashmapstring, " ");
    strcat(hashmapstring, name);
    strcat(hashmapstring, " ");
    for (int i = 0; i < HASHMAP_CAPACITY; i++)
    {
        HashElem* elem = hashmap[i];
        while (elem)
        {
            strcat(hashmapstring, elem->key);
            strcat(hashmapstring, ":");
            strcat(hashmapstring, elem->value);
            strcat(hashmapstring, ";");
            elem = elem->overflow_bucket;
        }
    }
    
    return hashmapstring;
}


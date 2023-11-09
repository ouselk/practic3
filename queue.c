

#include <stddef.h>
#include <stdlib.h>
#include "queue.h"
#include <string.h>

void queuePush(Queue* q, char* data)
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = strdup(data);
    newNode->next = NULL;
    
    if (q->head == NULL)
    {
        q->head = newNode;
        q->tail = newNode;
    }
    else
    {
        q->tail->next = newNode;
        q->tail = newNode;
    }
}

char* queuePop(Queue* q)
{
    if (q->head == NULL) {
        return NULL;
    }
    char* value = q->head->data;
    Node* tmp = q->head;
    q->head=q->head->next;
    if (q->head == NULL)
        q->tail = NULL;

    free(tmp);

    return value;
}
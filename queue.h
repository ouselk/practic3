#ifndef QUEUE_H
#define QUEUE_H

#include "node.h"

typedef struct Queue
{
    Node* head;
    Node* tail;
} Queue;

void queuePush(Queue* q, char* data);
char* queuePop(Queue* q);

#endif
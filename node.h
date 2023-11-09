#ifndef NODE_H
#define NODE_H

typedef struct Node
{
	char* data;
	struct Node* next;
} Node;

typedef struct ExNode
{
	char* data;
	struct ExNode* next;
    struct ExNode* previous;
} ExNode;

#endif
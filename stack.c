#include "stack.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

void stackPush(Node** head, char* data) {
    Node* tmp = (Node*)malloc(sizeof(Node));
    if (tmp == NULL) {
        exit(STACK_OVERFLOW);
    }
    tmp->next = *head;
    tmp->data = data;
    *head = tmp;
}

char* stackPop(Node** head) {
    Node* out;
    char* value;
    if (*head == NULL) {
        exit(STACK_UNDERFLOW);
    }
    out = *head;
    *head = (*head)->next;
    value = out->data;
    free(out);
    return value;
}
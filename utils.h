#ifndef UTILS_H
#define UTILS_H

Node* stackFromString(char* string);
char* stackInString(Node* stack, char* name);
Set** setFromString(char* string);
char* setInString(Set** set, char* name);
Queue* queueFromString(char* string);
char* queueInString(Queue* queue, char* name);
HashElem** hashmapFromString(char* string);
char* hashmapInString(HashElem** hashmap, char* name);

#endif
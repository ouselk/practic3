#ifndef FILE_H
#define FILE_H


int readFile(char filename[], char *str_result[]);
int writeIntoEndOfFile(char* filename, char* str);
char* readStructure(char* filename, char* name, char** typestruct);
int fileCheck(char* filename);

#endif
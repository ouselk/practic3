#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hashmap.h"
#include "set.h"
#include "queue.h"
#include "stack.h"
#include "file.h"


int fileCheck(char* filename)
{
    FILE* file = fopen(filename, "a");
    if (!file)
        return -1;
    fclose(file);

    file = fopen(filename, "r");
    if (!file)
        return -2;
    fclose(file);

    return 0;
}

int writeIntoEndOfFile(char* filename, char* str)
{
    FILE* file = fopen(filename, "a");
    if (!file)
        return -1;
    fprintf(file, "%s\n", str);
    fclose(file);
    return 0;
}

char* readStructure(char* filename, char* name, char** typeStruct)
{
    FILE* file = fopen(filename, "r");
    char typeOfStruct[2];
    char nameOfStruct[256];
    char contentStruct[1024];

    
    char* tempfilename = strdup(filename);
    strcat(tempfilename, ".tmp");
    FILE* tempfile = fopen(tempfilename, "w");
    //проходим до конца файла или пока не встретим нужную структуру,
    // попутно переписываем во временный файл
    while (fscanf(file, "%s""%s""%s" , typeOfStruct, nameOfStruct, contentStruct)!=EOF && 
            strcmp(nameOfStruct, name) != 0)
        fprintf(tempfile, "%s %s %s\n", typeOfStruct, nameOfStruct, contentStruct);

    //если не нашли структуру с этим именем возвращаем нуль
    if (strcmp(nameOfStruct, name) != 0)
    {    
        remove(tempfilename);
        fclose(tempfile);
        fclose(file);
        return NULL;
    }
    else // если нашли, дозаписываем во временный файл все остальное
    {
        char typeOfStruct[2];
        char nameOfStruct[256];
        char contentStruct[1024];
        while (fscanf(file, "%s""%s""%s" , typeOfStruct, nameOfStruct, contentStruct)!=EOF)
            fprintf(tempfile, "%s %s %s\n", typeOfStruct, nameOfStruct, contentStruct);
    }

    remove(filename);
    rename(tempfilename, filename);
    char* content = strdup(contentStruct);
    *typeStruct = strdup(typeOfStruct);
    fclose(file);
    fclose(tempfile);

    return content;
}

int readFile(char* filename, char** str_result)
{
    FILE* file = fopen(filename, "r");
    if (!file)
    {
        *str_result = NULL;
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    *str_result = malloc(sizeof(char) * size + 1);
    if (*str_result == NULL)
    {
        fclose(file);
        return 1;
    }

    (*str_result)[fread(*str_result, sizeof(char), size, file)] = '\0';

    fclose(file);
    return 0;
}
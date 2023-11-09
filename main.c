#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "queue.h"
#include "hashmap.h"
#include "set.h"
#include "file.h"
#include "utils.h"

#define FILENAME "bd.txt"
int main(int argc, char** argv)
{

    if (argc != 5 || strcmp(argv[1], "--file") != 0 || strcmp(argv[3], "--query") != 0)
    {
        printf("Use program:\n%s --file \"path of file\" --query \"query to DBMS\"\n", argv[0]);
        exit(-1);
    }

    char* filename = argv[2];
    char* request  = argv[4];

    if (fileCheck(filename) != 0)
    {
        printf("Open file error.\n");
        exit(-2);
    }

    char* command = strtok(request, " ");
    char* structname = strtok(NULL, " ");
    char* firstarg = strtok(NULL, " ");
    char* secondarg = strtok(NULL, " ");
    if (strtok(NULL, " "))
    {
        printf("Incorrect query.\n");
        exit(-3);
    }

    char* typeOfStruct = NULL;
    if (strcmp(command, "SADD") == 0)
    {
        char* stringstruct = readStructure(filename, structname, &typeOfStruct);
        if (!stringstruct && !typeOfStruct)
        {
            stringstruct = ";";
            typeOfStruct = "S";
        }
        else if (strcmp(typeOfStruct, "S")!=0)
        {
            printf("Incorrect operation for this type of struct.\n");
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, structname);
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, stringstruct);
            writeIntoEndOfFile(filename, typeOfStruct);
            exit(-4);
        }
        Set** set = setFromString(stringstruct);
        setAdd(set, firstarg);
        stringstruct = setInString(set, structname);
        writeIntoEndOfFile(filename, stringstruct);
    }
    else if (strcmp(command, "SREM") == 0)
    {
        char* stringstruct = readStructure(filename, structname, &typeOfStruct);
        if (!stringstruct && !typeOfStruct)
        {
            printf("Set with nane %s is not exists.\n", structname);
            exit(-666);
        }
        else if (strcmp(typeOfStruct, "S")!=0)
        {
            printf("Incorrect operation for this type of struct.\n");
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, structname);
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, stringstruct);
            writeIntoEndOfFile(filename, typeOfStruct);
            exit(-4);
        }
        Set** set = setFromString(stringstruct);
        setRem(set, firstarg);
        writeIntoEndOfFile(filename, setInString(set, structname));
    }
    else if (strcmp(command, "SISMEMBER") == 0)
    {
        char* stringstruct = readStructure(filename, structname, &typeOfStruct);
        if (!stringstruct && !typeOfStruct)
        {
            printf("Set with nane %s is not exists.\n", structname);
            exit(-666);
        }
        else if (strcmp(typeOfStruct, "S")!=0)
        {
            printf("Incorrect operation for this type of struct.\n");
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, structname);
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, stringstruct);
            writeIntoEndOfFile(filename, typeOfStruct);
            exit(-4);
        }
        Set** set = setFromString(stringstruct);
        bool isMember = setIsMember(set, firstarg);
        if (isMember)
            printf("%s is member of set %s.\n", firstarg, structname);
        else
            printf("%s is not member of set %s.\n", firstarg, structname);
        writeIntoEndOfFile(filename, setInString(set, structname));
    }


    else if (strcmp(command, "SPUSH") == 0)
    {
        char* stringstruct = readStructure(filename, structname, &typeOfStruct);
        printf("tos: %s\n", typeOfStruct);
        if (!stringstruct && !typeOfStruct)
        {
            stringstruct = "";
            typeOfStruct = "s";
        }
        else if (strcmp(typeOfStruct, "s")!=0)
        {
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, structname);
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, stringstruct);
            writeIntoEndOfFile(filename, typeOfStruct);
            printf("Incorrect operation for this type of struct.\n");
            exit(-4);
        }
        Node* stack = stackFromString(stringstruct);
        stackPush(&stack, firstarg);
        stringstruct = stackInString(stack, structname);
        writeIntoEndOfFile(filename, stringstruct);
    }
    else if (strcmp(command, "SPOP") == 0)
    {
        char* stringstruct = readStructure(filename, structname, &typeOfStruct);
        if (!stringstruct)
        {
            printf("Stack with name %s is not exists.\n", structname);
            exit(-666);
        }
        else if (strcmp(typeOfStruct, "s")!=0)
        {
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, structname);
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, stringstruct);
            writeIntoEndOfFile(filename, typeOfStruct);
            printf("Incorrect operation for this type of struct.\n");
            exit(-4);
        }
        Node* stack = stackFromString(stringstruct);
        char* poped = stackPop(&stack);
        printf("Poped elem: %s\n", poped);
        if (!stack)
            printf("Stack is empty.\n");
        writeIntoEndOfFile(filename, stackInString(stack, structname));
    }


    else if (strcmp(command, "QPUSH") == 0)
    {
        char* stringstruct = readStructure(filename, structname, &typeOfStruct);
        printf("tos: %s\n", typeOfStruct);
        if (!stringstruct && !typeOfStruct)
        {
            stringstruct = "";
            typeOfStruct = "q";
        }
        else if (strcmp(typeOfStruct, "q")!=0)
        {
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, structname);
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, stringstruct);
            writeIntoEndOfFile(filename, typeOfStruct);
            printf("Incorrect operation for this type of struct.\n");
            exit(-4);
        }
        Queue* queue = queueFromString(stringstruct);
        queuePush(queue, firstarg);
        stringstruct = queueInString(queue, structname);
        writeIntoEndOfFile(filename, stringstruct);
    }
    else if (strcmp(command, "QPOP") == 0)
    {
        char* stringstruct = readStructure(filename, structname, &typeOfStruct);
        if (!stringstruct)
        {
            printf("queue with name %s is not exists.\n", structname);
            exit(-666);
        }
        else if (strcmp(typeOfStruct, "q")!=0)
        {
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, structname);
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, stringstruct);
            writeIntoEndOfFile(filename, typeOfStruct);
            printf("Incorrect operation for this type of struct.\n");
            exit(-4);
        }
        Queue* queue = queueFromString(stringstruct);
        char* poped = queuePop(queue);
        printf("Poped elem: %s\n", poped);
        if (!queue)
            printf("queue is empty.\n");
        writeIntoEndOfFile(filename, queueInString(queue, structname));
    }


    else if (strcmp(command, "HSET") == 0)
    {
        char* stringstruct = readStructure(filename, structname, &typeOfStruct);
        if (!stringstruct && !typeOfStruct)
        {
            stringstruct = ":;";
            typeOfStruct = "h";
        }
        else if (strcmp(typeOfStruct, "h")!=0)
        {
            printf("Incorrect operation for this type of struct.\n");
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, structname);
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, stringstruct);
            writeIntoEndOfFile(filename, typeOfStruct);
            exit(-4);
        }
        HashElem** hashmap = hashmapFromString(stringstruct);
        hashmapAdd(hashmap, firstarg, secondarg);
        stringstruct = hashmapInString(hashmap, structname);
        writeIntoEndOfFile(filename, stringstruct);
    }
    else if (strcmp(command, "HDEL") == 0)
    {
        char* stringstruct = readStructure(filename, structname, &typeOfStruct);
        if (!stringstruct && !typeOfStruct)
        {
            printf("Hashmap with nane %s is not exists.\n", structname);
            exit(-666);
        }
        else if (strcmp(typeOfStruct, "h")!=0)
        {
            printf("Incorrect operation for this type of struct.\n");
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, structname);
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, stringstruct);
            writeIntoEndOfFile(filename, typeOfStruct);
            exit(-4);
        }
        HashElem** hashmap = hashmapFromString(stringstruct);
        hashmapDelete(hashmap, firstarg);
        writeIntoEndOfFile(filename, hashmapInString(hashmap, structname));
    }
    else if (strcmp(command, "HGET") == 0)
    {
        char* stringstruct = readStructure(filename, structname, &typeOfStruct);
        if (!stringstruct && !typeOfStruct)
        {
            printf("Hashmap with nane %s is not exists.\n", structname);
            exit(-666);
        }
        else if (strcmp(typeOfStruct, "h")!=0)
        {
            printf("Incorrect operation for this type of struct.\n");
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, structname);
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, stringstruct);
            writeIntoEndOfFile(filename, typeOfStruct);
            exit(-4);
        }
        HashElem** hashmap = hashmapFromString(stringstruct);
        char* value = hashmapGetValue(hashmap, firstarg);
        if (value)
            printf("Value %s by key %s.\n", value, firstarg);
        else
            printf("%s is not member of hashmap %s.\n", firstarg, structname);
        writeIntoEndOfFile(filename, hashmapInString(hashmap, structname));
    }
}
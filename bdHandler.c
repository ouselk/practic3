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
#include "socket.h"

char* bdHandler(int argc, char** argv)
{

    if (argc < 5 || strcmp(argv[1], "--file") != 0 || strcmp(argv[3], "--query") != 0)
    {
        char* str = malloc(sizeof(char) * MAX_ANSWER_SIZE);
        snprintf(str, MAX_ANSWER_SIZE, "Use program:\n%s --file \"path of file\" --query \"query to DBMS\"", argv[0]);
        return str;
    }

    char* filename = argv[2];

    if (fileCheck(filename) != 0)
    {
        return "Open file error.";
    }

    char* command = argv[4];
    char* structname = argv[5];
    char* firstarg = argv[6];
    char* secondarg = argv[7];


    char* typeOfStruct = NULL;
    if (strcmp(command, "SADD") == 0)
    {
        if (argc<=6)
            return "Incorrect query";
        char* stringstruct = readStructure(filename, structname, &typeOfStruct);
        if (!stringstruct && !typeOfStruct)
        {
            stringstruct = ";";
            typeOfStruct = "S";
        }
        else if (strcmp(typeOfStruct, "S")!=0)
        {
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, structname);
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, stringstruct);
            writeIntoEndOfFile(filename, typeOfStruct);
            return ("Incorrect operation for this type of struct.");
        }
        Set** set = setFromString(stringstruct);
        if (setIsMember(set, firstarg))
        {
            stringstruct = setInString(set, structname);
            writeIntoEndOfFile(filename, stringstruct);
            return "Element exists.";
        }
        setAdd(set, firstarg);
        stringstruct = setInString(set, structname);
        writeIntoEndOfFile(filename, stringstruct);
        return "Succefull SADD";
    }
    else if (strcmp(command, "SREM") == 0)
    {
        if (argc<=6)
            return "Incorrect query";
        char* stringstruct = readStructure(filename, structname, &typeOfStruct);
        if (!stringstruct && !typeOfStruct)
        {
            char* str = malloc(sizeof(char) * MAX_ANSWER_SIZE);
            snprintf(str, MAX_ANSWER_SIZE, "Set with nane %s is not exists.", structname);
            return str;
        }
        else if (strcmp(typeOfStruct, "S")!=0)
        {
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, structname);
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, stringstruct);
            writeIntoEndOfFile(filename, typeOfStruct);
            return "Incorrect operation for this type of struct.";
        }
        Set** set = setFromString(stringstruct);
        setRem(set, firstarg);
        writeIntoEndOfFile(filename, setInString(set, structname));
        return "Succefull SREM";
    }
    else if (strcmp(command, "SISMEMBER") == 0)
    {
        if (argc<=6)
            return "Incorrect query";
        char* stringstruct = readStructure(filename, structname, &typeOfStruct);
        if (!stringstruct && !typeOfStruct)
        {
            char* str = malloc(sizeof(char) * MAX_ANSWER_SIZE);
            snprintf(str, MAX_ANSWER_SIZE, "Set with name %s is not exists.\n", structname);
            return str;
        }
        else if (strcmp(typeOfStruct, "S")!=0)
        {
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, structname);
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, stringstruct);
            writeIntoEndOfFile(filename, typeOfStruct);
            return "Incorrect operation for this type of struct.";
        }
        Set** set = setFromString(stringstruct);
        bool isMember = setIsMember(set, firstarg);
        char* str = malloc(sizeof(char) * MAX_ANSWER_SIZE);
        if (isMember)
            snprintf(str, MAX_ANSWER_SIZE, "%s is member of set %s.\n", firstarg, structname);
        else
            snprintf(str, MAX_ANSWER_SIZE, "%s is not member of set %s.\n", firstarg, structname);
        return str;
        writeIntoEndOfFile(filename, setInString(set, structname));
    }


    else if (strcmp(command, "SPUSH") == 0)
    {
        if (argc<=6)
            return "Incorrect query";
        char* stringstruct = readStructure(filename, structname, &typeOfStruct);
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
            return ("Incorrect operation for this type of struct.\n");
        }
        Node* stack = stackFromString(stringstruct);
        stackPush(&stack, firstarg);
        stringstruct = stackInString(stack, structname);
        writeIntoEndOfFile(filename, stringstruct);
        return "Succefull SPUSH";
    }
    else if (strcmp(command, "SPOP") == 0)
    {
        if (argc<=5)
            return "Incorrect query";
        char* stringstruct = readStructure(filename, structname, &typeOfStruct);
        if (!stringstruct)
        {
            char* str = malloc(sizeof(char) * MAX_ANSWER_SIZE);
            snprintf(str, MAX_ANSWER_SIZE, "Stack with name %s is not exists.\n", structname);
            return str;
        }
        else if (strcmp(typeOfStruct, "s")!=0)
        {
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, structname);
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, stringstruct);
            writeIntoEndOfFile(filename, typeOfStruct);
            return ("Incorrect operation for this type of struct.\n");
        }
        Node* stack = stackFromString(stringstruct);
        char* poped = stackPop(&stack);
        char* str = malloc(sizeof(char) * MAX_ANSWER_SIZE);
        snprintf(str, MAX_ANSWER_SIZE, "Poped elem: %s", poped);
        writeIntoEndOfFile(filename, stackInString(stack, structname));
        if (!stack)
            strcat(str, "\nStack is empty.");
        return str;
    }


    else if (strcmp(command, "QPUSH") == 0)
    {
        if (argc<=6)
            return "Incorrect query";
        char* stringstruct = readStructure(filename, structname, &typeOfStruct);
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
            return "Incorrect operation for this type of struct.";
        }
        Queue* queue = queueFromString(stringstruct);
        queuePush(queue, firstarg);
        stringstruct = queueInString(queue, structname);
        writeIntoEndOfFile(filename, stringstruct);
        return "Succefull QPUSH";
    }
    else if (strcmp(command, "QPOP") == 0)
    {
        if (argc<=5)
            return "Incorrect query";
        char* stringstruct = readStructure(filename, structname, &typeOfStruct);
        if (!stringstruct)
        {
            char* str = malloc(sizeof(char) * MAX_ANSWER_SIZE);
            snprintf(str, MAX_ANSWER_SIZE, "queue with name %s is not exists.", structname);
            return str;
        }
        else if (strcmp(typeOfStruct, "q")!=0)
        {
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, structname);
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, stringstruct);
            writeIntoEndOfFile(filename, typeOfStruct);
            return "Incorrect operation for this type of struct.\n";
        }
        Queue* queue = queueFromString(stringstruct);
        char* poped = queuePop(queue);
        char* str = malloc(sizeof(char) * MAX_ANSWER_SIZE);
        snprintf(str, MAX_ANSWER_SIZE, "Poped elem: %s", poped);
        if (!queue)
            strcat(str, "\nqueue is empty.");
        writeIntoEndOfFile(filename, queueInString(queue, structname));
        return str;
    }


    else if (strcmp(command, "HSET") == 0)
    {
        if (argc<=7)
            return "Incorrect query";
        char* stringstruct = readStructure(filename, structname, &typeOfStruct);
        if (!stringstruct && !typeOfStruct)
        {
            stringstruct = ":;";
            typeOfStruct = "h";
        }
        else if (strcmp(typeOfStruct, "h")!=0)
        {
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, structname);
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, stringstruct);
            writeIntoEndOfFile(filename, typeOfStruct);
            return ("Incorrrect operation for this type of struct.");

        }
        HashElem** hashmap = hashmapFromString(stringstruct);
        hashmapAdd(hashmap, firstarg, secondarg);
        stringstruct = hashmapInString(hashmap, structname);
        writeIntoEndOfFile(filename, stringstruct);
        return "Succefull HSET";
    }
    else if (strcmp(command, "HDEL") == 0)
    {
        if (argc<=6)
            return "Incorrect query";
        char* stringstruct = readStructure(filename, structname, &typeOfStruct);
        if (!stringstruct && !typeOfStruct)
        {
            char* str = malloc(sizeof(char) * MAX_ANSWER_SIZE);
            snprintf(str, MAX_ANSWER_SIZE, "Hashmap with nane %s is not exists.\n", structname);
            return str;
        }
        else if (strcmp(typeOfStruct, "h")!=0)
        {
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, structname);
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, stringstruct);
            writeIntoEndOfFile(filename, typeOfStruct);
            return ("Incorrect operation for this type of struct.");
        }
        HashElem** hashmap = hashmapFromString(stringstruct);
        hashmapDelete(hashmap, firstarg);
        writeIntoEndOfFile(filename, hashmapInString(hashmap, structname));
        return "Succefull HDEL";
    }
    else if (strcmp(command, "HGET") == 0)
    {
        if (argc<=6)
            return "Incorrect query";
        char* stringstruct = readStructure(filename, structname, &typeOfStruct);
        if (!stringstruct && !typeOfStruct)
        {
            char* str = malloc(sizeof(char) * MAX_ANSWER_SIZE);
            snprintf(str, MAX_ANSWER_SIZE, "Hashmap with name %s is not exists.", structname);
            return str;
        }
        else if (strcmp(typeOfStruct, "h")!=0)
        {
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, structname);
            strcat(typeOfStruct, " ");
            strcat(typeOfStruct, stringstruct);
            writeIntoEndOfFile(filename, typeOfStruct);
            return ("Incorrect operation for this type of struct.");
        }
        HashElem** hashmap = hashmapFromString(stringstruct);
        char* value = hashmapGetValue(hashmap, firstarg);
        writeIntoEndOfFile(filename, hashmapInString(hashmap, structname));
        char* str = malloc(sizeof(char) * MAX_ANSWER_SIZE);
        if (value)
            snprintf(str, MAX_ANSWER_SIZE, "Value %s by key %s.\n", value, firstarg);
        else
            snprintf(str, MAX_ANSWER_SIZE, "%s is not member of hashmap %s.\n", firstarg, structname);
        return str;
    }
    else
        return "No exists such command.";

    return "Nobody never see that.";
}
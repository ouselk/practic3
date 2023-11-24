#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "socket.h"

#define PORT 6379
#define MAX_USERSTRING_LENGTH 512 

char *readStringFromUser()
{
    fflush(stdin);
    char *input = (char *)malloc(MAX_USERSTRING_LENGTH * sizeof(char));

    if (input == NULL) {
        return NULL;
    }

    fgets(input, MAX_USERSTRING_LENGTH, stdin);

    size_t length = strlen(input);
    if (length > 0 && input[length - 1] == '\n') {
        input[length - 1] = '\0';
    }

    return input;
}
int main(int argc, char** argv)
{
    if (argc<5) // необходимо минимум 5 аргументов для отправки корректного начального запроса на сервер
    {
        printf("Need 5 arguments");
        exit(EXIT_FAILURE);
    }
    int fd = Socket(AF_INET, SOCK_STREAM, 0); //создаем tcp ip сокет
    struct sockaddr_in addr = {0}; 
    addr.sin_family = AF_INET; //устанавливаем семейство протоколов ipv4
    addr.sin_port = htons(PORT); //устанавливаем порт, преобразуя его в порядок байтов для tcp/ip
    Inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr); // преобразуем 127.0.0.1 в байты и устанавливаем как адресс
    Connect(fd, (struct sockaddr*) &addr, sizeof addr); // соединяемся с сервером

    char request[MAX_USERSTRING_LENGTH] = {'\0'};
    for (int i = 0; i < argc; i++) //составляем запрос из аргументов с командой строки
    {
        strcat(request, argv[i]);
        strcat(request, " ");
    }
    write(fd, request, strlen(request)); // отправляем запрос
    char res[MAX_ANSWER_SIZE] = {'\0'};

    read(fd, res, MAX_ANSWER_SIZE); //считываем результат
    printf("server answer: %s\n", res);

    char* req = NULL;
    while (1) // в цикле запрашиваем и отправляем запрос
              // для указанного изначально фалйа
    {
        for (int i = 0; i < MAX_USERSTRING_LENGTH; i++)
            request[i] = '\0';
        for (int i = 0; i < MAX_ANSWER_SIZE; i++)
            res[i] = '\0';
       printf("Enter request: ");
        req = readStringFromUser();
        snprintf(request, MAX_USERSTRING_LENGTH, "%s %s %s %s %s", argv[0], "--file", argv[2], "--query", req);
        printf("%s\n", request);
        write(fd, request, strlen(request));
        read(fd, res, MAX_ANSWER_SIZE);
        printf("server answer: %s\n", res);
        if (strcmp(res, "STOP")==0)
            break;
    }

    close(fd);
}
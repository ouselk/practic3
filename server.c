#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "socket.h"
#include "bdHandler.h"

#define MAX_REQUEST_BYTES 256
#define PORT 6379


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void *clientHandler(void *socket_desc)
{
  // отслеживаем номер подключения для логов,
  // так как номер - общий для потоков ресурс,
  // используем мьютексы
  static int CONNECTION_NUM = 0;
  pthread_mutex_lock(&mutex);
  CONNECTION_NUM++;
  pthread_mutex_unlock(&mutex);
  int connection_num = CONNECTION_NUM;

  printf("New connection №%d\n", connection_num);

  int fd = *(int*)socket_desc;
  while (1)
  {
    char command[MAX_REQUEST_BYTES] = {'\0'};
    //считываем отправленные клиентом данные в строку command
    ssize_t nread = read(fd, command, MAX_REQUEST_BYTES);
    if (nread == -1)
    {
      perror("Read error");
      break;
    }
    if (nread == 0)
    {
      printf("End of file\n");
      break;
    }
    printf("User data of connection№%d: %s\n", connection_num, command);

    // раскладываем отправленное пользователем на аргументы для
    // функции bdhandler
    int argc = 0;
    char* nameofprogram = strtok(command, " ");
    if (nameofprogram)
      argc++;
    char* hyphypfile = strtok(NULL, " ");
    if (hyphypfile)
      argc++;
    char* filename = strtok(NULL , " ");
    if (filename)
      argc++;
    char* hyphypquery = strtok(NULL, " ");
    if (hyphypfile)
      argc++;
    char* query = strtok(NULL, " ");
    if (query)
      argc++;
    char* tablename = strtok(NULL, " ");
    if (tablename)
      argc++;
    char* firstarg = strtok(NULL, " ");
    if (firstarg)
      argc++;
    char* secondarg = strtok(NULL, " ");
    if (secondarg)
      argc++;
    
    char** argv = malloc(sizeof(char*) * argc);
    if (argc>0)
      argv[0] = nameofprogram;
    if (argc>1)
      argv[1] = hyphypfile;
    if (argc>2)
      argv[2] = filename;
    if (argc>3)
      argv[3] = hyphypquery;
    if (argc>4)
      argv[4] = query;
    if (argc>5)
      argv[5] = tablename;
    if (argc>6)
      argv[6] = firstarg;
    if (argc>7)
      argv[7] = secondarg;

    if (query && strcmp(query, "STOP") == 0)
    {
      write(fd, "STOP", 4);
      break;
    }
    // в ответ пишем возвращенную из функции bdHandler строку,
    // так как функция работает с файлом - общим для потоков ресурсом,
    // используем мьютексы.
    pthread_mutex_lock(&mutex);
//    char* answer =  bdHandler(argc, argv);
    write(fd,bdHandler(argc, argv), MAX_ANSWER_SIZE);
    //free(answer);
    pthread_mutex_unlock(&mutex);
  } 

  //закрываем сокет подключения и освобождаем память,
  //закрываем поток.
  close(fd);
  free(socket_desc);
  printf("Connection №%d processed succefuly, closed.\n", connection_num);
  pthread_exit(NULL);
}

int main()
{
  int socket = Socket(AF_INET, SOCK_STREAM, 0);//tcp/ip сокет

  struct sockaddr_in name = {0};
  name.sin_family = AF_INET; // семейство ipv4
  name.sin_port = htons (PORT); //устанавливаем порт, преобразуя его в порядок байтов для tcp/ip
  name.sin_addr.s_addr = INADDR_ANY; //указываем все доступные интерфейсы
  socklen_t addrlen = sizeof name;
  Bind(socket, (struct sockaddr *) &name, sizeof (name)); //привязываем сокет к имени
  Listen(socket, 5);  //прослушиваем сокет (максимальная очередь клиентов - 5)
  int fd = 0;
  // в цикле постоянно запрашиваем дескриптор для нового подключения,
  // когда оно происходит, отправляем дескриптор в функцию clientHandler
  // в новом потоке.
  while ((fd =  Accept(socket, (struct sockaddr* ) &name, &addrlen)))
  {
    pthread_t thread = 0;
    int *new_connection = malloc(sizeof(int));
    *new_connection = fd;
    if (pthread_create(&thread, NULL, clientHandler, (void*)new_connection) < 0)
    {
      perror("Error thread create");
      exit(EXIT_FAILURE);
    }
  }

  close(socket);
  close(fd);
}
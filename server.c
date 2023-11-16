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
    ssize_t nread = read(fd, command, MAX_REQUEST_BYTES);
    if (nread == -1)
    {
      perror("read error");
      break;
    }
    if (nread == 0)
    {
      printf("End of file");
      break;
    }
    printf("User data of connection№%d: %s\n", connection_num, command);

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

    char answer[MAX_ANSWER_SIZE] = {'\0'};
    if (strcmp(query, "STOP") == 0)
    {
      write(fd, "STOP", 4);
      break;
    }
    
    pthread_mutex_lock(&mutex);
    write(fd, bdHandler(argc, argv), MAX_ANSWER_SIZE);
    pthread_mutex_unlock(&mutex);
  } 
  free(socket_desc);
  printf("Connection №%d processed succefuly, closed.\n", connection_num);
  pthread_exit(NULL);

}

int main()
{
  int socket = Socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in name = {0};
  name.sin_family = AF_INET;
  name.sin_port = htons (PORT);
  name.sin_addr.s_addr = INADDR_ANY;
  socklen_t addrlen = sizeof name;
  Bind(socket, (struct sockaddr *) &name, sizeof (name));
  Listen(socket, 5);
  int fd;
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
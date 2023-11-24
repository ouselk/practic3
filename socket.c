#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "socket.h"

//Функции обертки, обрабатывающие ошибку при вызове
//оригинальной функции.
int Socket(int domain, int type, int protocol)
{
  int sock = -1;

  /* Create the socket. */
  sock = socket (domain, type, protocol);
  if (sock < 0)
    {   
      perror ("socket");
      exit (EXIT_FAILURE);
    }

  return sock;
}


void Listen(int socket, int n)
{
    int res = listen(socket, n);
    if (res==-1)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

}
void Bind (int socket, const struct sockaddr *addr, socklen_t addrlen)
{
  if (bind (socket, addr, addrlen) < 0)
  {
    perror("bind");
    exit(EXIT_FAILURE);
  }
}
int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{

  int res = accept(sockfd, addr, addrlen);
  if (res == -1)
  {
    perror("accept");
    exit(EXIT_FAILURE);
  }
  return res;
}

void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    int res = connect(sockfd, addr, addrlen);
    if (res == -1)
    {
        perror("connect error");
        exit(EXIT_FAILURE);
    }
}

void Inet_pton(int af, const char *src, void *dst)
{
    int res = inet_pton(af, src, dst);
    if (res==0)
    {
        printf("src does not contain a character"
        "string representing a valid network address"
       "in  the  specified address family\n");
       exit(EXIT_FAILURE);
    }
    if (res==-1)
    {
        perror("af does not contain a valid address family");
        exit(EXIT_FAILURE);
    }
}
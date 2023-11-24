#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>

#define MAX_ANSWER_SIZE 1024

 
int Socket(int domain, int type, int protocol);
void Listen(int socket, int n);
void Bind (int socket, const struct sockaddr *addr, socklen_t addrlen);
int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
void Inet_pton(int af, const char *src, void *dst);

#endif
//
// Created by bolvvv on 2021/5/20.
//
#ifndef BOLSOCKET_WRAP_H
#define BOLSOCKET_WRAP_H
#include <sys/socket.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <netinet/in.h>
#include<arpa/inet.h>
#include <cctype>
#include <sys/wait.h>
#include <strings.h>
#include <csignal>
void perr_exit(const char *s);
int Accept(int fd, struct sockaddr_in *sa, socklen_t *salenptr);
int Bind(int fd, const struct sockaddr_in *sa, socklen_t salen);
int Connect(int fd, const struct sockaddr_in *sa, socklen_t salen);
int Listen(int fd, int backlog);
int Socket(int family, int type, int protocol);
ssize_t Read(int fd, void *ptr, size_t nbytes);
ssize_t Write(int fd, const void *ptr, size_t nbytes);
int Close(int fd);
ssize_t Readn(int fd, void *vptr, size_t n);
ssize_t Writen(int fd, const void *vptr, size_t n);
ssize_t my_read(int fd, char *ptr);
ssize_t Readline(int fd, void *vptr, size_t maxlen);
#endif //BOLSOCKET_WRAP_H
//
// Created by bolvvv on 2021/5/20.
//

#include "wrap.h"


void perr_exit(const char *s) {
    perror(s);
    exit(-1);
}

int Accept(int fd, struct sockaddr_in *sa, socklen_t *salenptr) {
    int accept_result = accept(fd, (struct sockaddr *)sa, salenptr);
    if(accept_result < 0){
        perr_exit("accept error");
    }
    return accept_result;
}

int Bind(int fd, const struct sockaddr_in *sa, socklen_t salen) {
    int bind_result = bind(fd, (struct sockaddr*)sa, salen);
    if(bind_result < 0){
        perr_exit("bind error");
    }
    return bind_result;
}

int Connect(int fd, const struct sockaddr_in *sa, socklen_t salen) {
    int connect_result = connect(fd, (struct sockaddr*)sa, salen);
    if(connect_result < 0){
        perr_exit("connect error");
    }
    return connect_result;
}

int Listen(int fd, int backlog) {
    int listen_result = listen(fd, backlog);
    if(listen_result < 0){
        perr_exit("listen error");
    }
    return listen_result;
}

int Socket(int family, int type, int protocol) {
    int socket_result = socket(family, type, protocol);
    if(socket_result < 0){
        perr_exit("socket error");
    }
    return socket_result;
}

ssize_t Read(int fd, void *ptr, size_t nbytes) {
    int read_result = read(fd, ptr, nbytes);
    if(read_result < 0){
        perr_exit("read error");
    }
    return read_result;
}

ssize_t Write(int fd, const void *ptr, size_t nbytes) {
    int write_result = write(fd, ptr, nbytes);
    if(write_result < 0){
        perr_exit("write error");
    }
    return write_result;
}

int Close(int fd) {
    int close_result = close(fd);
    if(close_result < 0){
        perr_exit("close error");
    }
    return close_result;
}







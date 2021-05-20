//
// Created by bolvvv on 2021/5/18.
//
#include <sys/types.h>
#include <sys/socket.h>
#include <cstdio>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERV_PORT 9527

int client(){
    //创建socket
    int cfd = 0;
    cfd = socket(PF_INET, SOCK_STREAM, 0);
    if(cfd == -1){
        perror("socket error");
    }
    //初始化服务器的socket
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = PF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    inet_pton(PF_INET, "127.0.0.1", &serv_addr.sin_addr.s_addr);

    //connect操作
    int connect_res = connect(cfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(connect_res == -1){
        perror("connect error");
    }

    //write操作
    char buf[1024];
    write(cfd, "hello", 5);
    int ret = read(cfd, buf, sizeof(buf));
    write(STDOUT_FILENO, buf, ret);

    close(cfd);

}
int main(){
    client();
}
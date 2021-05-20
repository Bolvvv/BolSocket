//
// Created by bolvvv on 2021/5/20.
//
#include <sys/socket.h>
#include <cstdio>
#include <netinet/in.h>
#include<arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <iostream>
#include <cstdio>
int multi_process_server(){
    //建立socket
    int lfd = 0;
    lfd = socket(PF_INET, SOCK_STREAM, 0);
    if (lfd == -1){
        perror("socket build error");
    }

    //绑定socket与socket_in结构体
    sockaddr_in addr_in;
    addr_in.sin_family = PF_INET;
    addr_in.sin_port = htons(9527);
    addr_in.sin_addr.s_addr = htonl(INADDR_ANY);
    int bind_result = bind(lfd, (struct sockaddr *)&addr_in, sizeof(addr_in));
    if(bind_result == -1){
        perror("bind error");
    }

    //listen
    int listen_result = listen(lfd, 100);
    if(listen_result == -1){
        perror("listen error");
    }
    while(1){
        //accept
        sockaddr_in sock_clit;//用来接收客户端的连接的sock
        socklen_t cli_sock_size = sizeof(sock_clit);
        int cfd = accept(lfd, (struct sockaddr *)&sock_clit, &cli_sock_size);
        if(cfd == -1){
            perror("accept error");
        }
        int pid = fork();
        if (pid == 0){
            //如果是子进程自己
            close(lfd);//关闭之前代码生成的server端的lfd
            //显示连接信息：
            char client_IP[1024];
            std::cout<<"client ip:"<<inet_ntop(PF_INET, &sock_clit.sin_addr.s_addr, client_IP, sizeof (client_IP))
                     <<" port:"<<ntohs(sock_clit.sin_port)<<std::endl;

            char buffer[1024];
            int read_result = read(cfd, buffer, sizeof (buffer));
            if(read_result == -1){
                perror("read error");
            }
            //将小字母变为大字母
            for(char & i : buffer){
                i = toupper(i);
            }
            write(cfd, buffer, sizeof (buffer));
        }
        else if(pid > 0){
            //为父进程，仅仅只需要做子进程回收
            close(cfd);
            while (1){
                waitpid(0, NULL, WNOHANG);
                continue;
            }
        }
    }
}
int main(){
    multi_process_server();
}
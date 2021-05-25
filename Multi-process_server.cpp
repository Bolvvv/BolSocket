//
// Created by bolvvv on 2021/5/20.
//
#include "wrap.h"

void catch_child(int signum){
    while((waitpid(0, nullptr, WNOHANG)) > 0);
}

int multi_process_server(){
    //需要用到的参数
    pid_t pid;//进程pid
    int ret;//读入数据长度
    char buffer[1024];//缓冲区

    //建立服务器socket
    int lfd = Socket(PF_INET, SOCK_STREAM, 0);

    sockaddr_in srv_addr{}, clt_addr{};//建立服务器与客户端的addr
    socklen_t cli_sock_size = sizeof(clt_addr);//客户端addr的长度
    //绑定socket与socket_in结构体
    bzero(&srv_addr, sizeof (srv_addr));//将地址结构清零
    srv_addr.sin_family = PF_INET;
    srv_addr.sin_port = htons(9528);
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    Bind(lfd, &srv_addr, sizeof(srv_addr));

    //listen
    Listen(lfd, 100);

    while(true){
        //accept
        int cfd = Accept(lfd, &clt_addr, &cli_sock_size);
        pid = fork();
        if (pid == 0){
            //如果是子进程自己
            close(lfd);//关闭之前代码生成的server端的lfd
            //显示连接信息：
            char client_IP[1024];
            std::cout<<"client ip:"<<inet_ntop(PF_INET, &clt_addr.sin_addr.s_addr, client_IP, sizeof (client_IP))
                     <<" port:"<<ntohs(clt_addr.sin_port)<<std::endl;
            ret = Read(cfd, buffer, sizeof (buffer));
            if(ret == 0){
                close(cfd);
                exit(1);
            }
            //将小字母变为大字母
            for(char & i : buffer){
                i = toupper(i);
            }
            Write(cfd, buffer, ret);
            Write(STDOUT_FILENO, buffer, ret);

        }
        else if(pid > 0){
            //为父进程，仅仅只需要做子进程回收
            struct sigaction act{};

            act.sa_handler = catch_child;
            sigemptyset(&act.sa_mask);
            act.sa_flags = 0;
            ret = sigaction(SIGCHLD, &act, nullptr);
            if (ret!=0){
                perr_exit("sigaction error");
            }

            close(cfd);
            continue;
        } else{
            perr_exit("fork error");
        }
    }
}
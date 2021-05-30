//
// Created by bolvvv on 2021/5/27.
//

#include "../include/wrap.h"
#include <sys/epoll.h>

#define BUF_SIZE 1024
#define SERV_PORT 9527

[[noreturn]]void multi_io_epoll_server(){
    int lfd = socket(AF_INET, SOCK_STREAM, 0);//创建监听套接字
    int opt = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt);//创建端口复用

    //创建服务器addr
    sockaddr_in serv_addr{}, cli_addr{};
    socklen_t serv_addr_len = sizeof serv_addr;
    socklen_t cli_addr_len = sizeof cli_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bzero(&serv_addr, serv_addr_len);
    //Bind&Listen
    Bind(lfd, &serv_addr, serv_addr_len);
    Listen(lfd, 128);

    //实现epoll
    int epfd = epoll_create(1024);//epfd，是描述epoll实例（红黑树）的文件描述符，亦即该红黑树的树根
    if(epfd == -1) perr_exit("epoll_create error");
    struct epoll_event tep{}, ep[1024];//用于设置单个文件描述符以及epoll返回的数组

    tep.events = EPOLLIN;tep.data.fd = lfd;//指定lfd的监听事件为读

    int res = epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &tep);//将lfd添加到epoll实例的监听中
    if(res == -1) perr_exit("epoll_ctl error");
    while(true){
        int ret = epoll_wait(epfd, ep, 1024, -1);
        for(int i = 0; i < ret; ++i){
            if(!(ep[i].events & EPOLLIN)) continue;
            if(ep[i].data.fd == lfd){
                //当在ep中查找到了lfd文件描述符，表示有客户端触发了监听事件，客户端请求建立连接，因此将此客户端纳入epoll的监听实例中
                int cfd = Accept(lfd, &cli_addr, &cli_addr_len);
                //显示连接信息：
                char client_IP[1024];
                std::cout<<"client ip:"<<inet_ntop(AF_INET, &cli_addr.sin_addr.s_addr, client_IP, sizeof (client_IP))
                         <<" port:"<<ntohs(cli_addr.sin_port)<<std::endl;
                tep.data.fd = cfd;
                tep.events = EPOLLIN;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &tep);
            } else{
                char buffer[BUF_SIZE];
                //此时表明其他客户端中发起了读写事件请求
                int read_num = Read(ep[i].data.fd, buffer, sizeof buffer);
                if(read_num == 0){
                    epoll_ctl(epfd, EPOLL_CTL_DEL, ep[i].data.fd, NULL);//从epoll实例中删除该客户端的文件描述符
                    Close(ep[i].data.fd);//读到的数据为0，说明客户端发来关闭连接请求
                } else if(read_num > 0){
                    for(int j = 0; j < read_num; ++j) buffer[j] = toupper(buffer[j]);
                    Write(ep[i].data.fd, buffer, read_num);
                } else{
                    perr_exit("read error");
                }
            }
        }
    }
    Close(lfd);
}
//int main(){
//    multi_io_epoll_server();
//}
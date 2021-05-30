//
// Created by bolvvv on 2021/5/25.
//
#include "../include/wrap.h"

#define BUFSIZ 1024
#define SERV_PORT 9527

[[noreturn]] void multi_io_switch_server(){
    int i, j, n, nready;
    int maxfd = 0;
    int listenfd, connfd;
    char buf[BUFSIZ];
    struct sockaddr_in clie_addr, serv_addr;
    socklen_t clie_addr_len;
    listenfd = Socket(PF_INET, SOCK_STREAM, 0);
    //端口复用
    int opt = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    //addr清零
    bzero(&serv_addr, sizeof serv_addr);

    serv_addr.sin_family=PF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERV_PORT);
    Bind(listenfd, &serv_addr, sizeof(serv_addr));
    Listen(listenfd, 128);

    //------------以下代码就实现了使用select完成多路IO选择服务器
    fd_set rset, allset;//rset读事件文件描述符集合，allset用来暂存
    maxfd = listenfd;//存储最大的文件描述符
    FD_ZERO(&allset);//将暂存集合清零
    FD_SET(listenfd, &allset);//构造select监控文件描述符集

    while(1){
        rset = allset;
        nready = select(maxfd+1, &rset, nullptr, nullptr, nullptr);
        if(nready < 0)
            perr_exit("select error");
        if(FD_ISSET(listenfd, &rset)){//有新的客户端连接请求，在这一步，rset会被修改
            clie_addr_len = sizeof clie_addr;
            connfd = Accept(listenfd, &clie_addr, &clie_addr_len);//Accept不会阻塞，因为有select的存在，当调用Accept时已经可以确定有连接请求了
            FD_SET(connfd, &allset);//向监控文件描述符集合allset添加新的文件描述符connfd

            if(maxfd < connfd) maxfd = connfd;
            if(0 == --nready) continue;//只有listenfd有事件，后续的for不需要执行
        }
        //如果nready大于1，说明不只有lfd的连接建立请求，还存在其他的连接操作，在这个demo里面就是客户端有写操作，因此服务端需要进行读操作
        for(i = listenfd+1; i <= maxfd; i++){
            if(FD_ISSET(i, &rset)){
                if((n = Read(i, buf, sizeof buf)) == 0){
                    Close(i);//当client关闭链接时，服务器端也关闭对应连接
                    FD_CLR(i, &allset);
                } else if(n > 0){
                    for(j=0; j < n; j++) buf[j] = toupper(buf[j]);
                    Write(i, buf, n);
                }
            }
        }
    }
    Close(listenfd);
}
//int main(){
//    multi_io_switch_server();
//}
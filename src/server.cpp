#include <sys/socket.h>
#include <cstdio>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/uio.h>
#include <unistd.h>
#include <ctype.h>
#include <iostream>
#define SERV_PORT 9527

int server(){
    //socket 操作
    int lfd = 0;
    lfd = socket(PF_INET, SOCK_STREAM, 0);
    if(lfd == -1){
        perror("socket error");
    }

    //初始化serv_addr和数据结构
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = PF_INET;//设定socket的协议
    serv_addr.sin_port = htons(SERV_PORT);//host to network short，将主机的无符号短整型数转换为网络字节序
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);//host to network long，将主机的无符号长整型数转换为网络字节序

    //bind 操作
    int bind_res = bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(bind_res == -1){
        perror("bind error");
    }

    //listen 操作
    int listen_res = listen(lfd, 128);
    if(listen_res == -1){
        perror("listen error");
    }

    //初始化accept需要用到的用于接收client端的新socket
    struct sockaddr_in clit_addr;//这一步不需要初始化，由客户端传来的socket尽行初始化，是一个传入参数
    socklen_t clit_addr_len = sizeof(clit_addr);

    //accept 操作
    int cfd = accept(lfd, (struct sockaddr *)& clit_addr, &clit_addr_len);//返回的accept_res为socket的描述符
    if(cfd == -1){
        perror("accept error");
    }
    char client_IP[1024];
    std::cout<<"client ip:"<<inet_ntop(PF_INET, &clit_addr.sin_addr.s_addr, client_IP, sizeof (client_IP))
    <<"port:"<<ntohs(clit_addr.sin_port)<<std::endl;
    //read操作
    char buf[BUFSIZ];//初始化一个读取client的buf, 大小为4096
    int ret_size;
    ret_size= read(cfd, buf, sizeof (buf));//返回实际读到的字节数
    if (ret_size == -1){
        perror("read error");
    }
    write(STDOUT_FILENO, buf, ret_size);//把数据写出到屏幕上

    //完成此次通信的功能：将小写字母转换为大写字母
    for(int i =0; i < ret_size; i++){
        buf[i] = toupper(buf[i]);
    }

    //write操作,将buffer的值写回到客户端
    write(cfd, buf, ret_size);
    return 0;

}
int main(){
    server();
}
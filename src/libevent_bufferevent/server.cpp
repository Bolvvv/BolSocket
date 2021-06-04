//
// Created by bolvvv on 2021/6/4.
//

#include "wrap.h"

#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <cstring>

#define SERV_PORT 9527

void read_cb(struct bufferevent *bev, void *ctx){
    char buffer[1024];
    bufferevent_read(bev, buffer, sizeof buffer);
    std::cout<<"读取到数据："<<buffer<<std::endl;
    char *p = "server: receive client info";
    bufferevent_write(bev, p, strlen(p)+1);//额外添加
    sleep(1);
}

void write_cb(struct bufferevent *bev, void *arg){
    std::cout<<"I am server, has written data to client"<<std::endl;
}

void event_cb(struct bufferevent *bev, short events, void *arg){
    if(events & BEV_EVENT_EOF){
        std::cout<<"connection closed"<<std::endl;
    } else if(events & BEV_EVENT_ERROR){
        std::cout<<"some other error"<<std::endl;
    }
    bufferevent_free(bev);
    std::cout<<"source has been free"<<std::endl;
}
void listener_cb(struct evconnlistener * listener, evutil_socket_t fd, struct sockaddr * addr, int socklen, void *user_arg){
    auto *base = (event_base *)user_arg;
    struct bufferevent * buff_event = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    bufferevent_setcb(buff_event, read_cb, write_cb, event_cb, nullptr);
    bufferevent_enable(buff_event, EV_READ|EV_WRITE);
}
void bufferevent_server(){
    //配置服务器基础信息
    sockaddr_in serv_addr{};
    socklen_t serv_addr_len = sizeof serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bzero(&serv_addr, serv_addr_len);
    //创建event_base
    struct event_base *base = event_base_new();
    //创建服务器监听器
    struct evconnlistener *ev_listener = evconnlistener_new_bind(
            base, listener_cb, base, LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE, -1, (struct sockaddr *)&serv_addr,serv_addr_len);
    event_base_dispatch(base);
    evconnlistener_free(ev_listener);
}

int main(){
    bufferevent_server();
}
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include "event2/event.h"
#include "wrap.h"

void write_cb(evutil_socket_t fd, short what, void *arg){
    char buff[1024] = {0};
    static int num = 0;
    sprintf(buff, "hello,world-%d\n", num++);
    write(fd, buff, strlen(buff)+1);
    sleep(1);
}

void write_pipe(){
    int fd = open("myfifo", O_WRONLY|O_NONBLOCK);
    if(fd == -1) perr_exit("open error");

    /*--------调用libevent进行事件监听-----*/

    //创建基础event_base
    struct event_base *base = event_base_new();;//创建event_base
    //创建监听事件
    event * write_event = event_new(base, fd, EV_WRITE | EV_PERSIST, write_cb, nullptr);
    //将事件添加到event_base上
    event_add(write_event, nullptr);
    //事件循环
    event_base_dispatch(base);

    //结束后，释放资源
    event_free(write_event);
    event_base_free(base);
    close(fd);
}
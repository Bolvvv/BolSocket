#include "wrap.h"
#include <sys/stat.h>
#include <fcntl.h>
#include "event2/event.h"

void read_cb(evutil_socket_t fd, short what, void *arg){
    char buf[1024] = {0};
    int len = read(fd, buf, sizeof buf);
    const char * result = what&EV_READ ? "yes":"No";
    std::cout<<"read event: "<<result<<std::endl;
    std::cout<<"data len = "<<len<<" buf = "<<buf<<std::endl;
}

void read_pipe(){
    //建立有名管道
    unlink("myfifo");
    mkfifo("myfifo", 0664);
    int fd  = open("myfifo", O_RDONLY|O_NONBLOCK);
    if(fd == -1) perr_exit("open error");

    /*--------调用libevent进行事件监听-----*/

    //创建基础event_base
    struct event_base *base = event_base_new();;//创建event_base
    //创建监听事件
    event * read_event = event_new(base, fd, EV_READ | EV_PERSIST, read_cb, nullptr);
    //将事件添加到event_base上
    event_add(read_event, nullptr);
    //事件循环
    event_base_dispatch(base);

    //结束后，释放资源
    event_free(read_event);
    event_base_free(base);
    close(fd);
}
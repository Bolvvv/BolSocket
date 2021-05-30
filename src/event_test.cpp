#include "../include/wrap.h"
#include <event2/event.h>


void test_event(){
    int i;
    struct event_base *base = event_base_new();
    const char **buf;
    buf = event_get_supported_methods();

    for(i = 0; i < 10; i++){
        std::cout<<buf[i]<<std::endl;
    }
}
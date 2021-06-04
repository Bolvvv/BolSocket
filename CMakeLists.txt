cmake_minimum_required(VERSION 3.10)
project(BolSocket)
set(CMAKE_CXX_STANDARD 14)

set(LINK_DIR /usr/local/lib)
set(INC_DIR /usr/local/include)

include_directories("./include")
include_directories(${INC_DIR})
link_directories(${LINK_DIR})

add_executable(BolSocket2 main.cpp src/wrap.cpp src/libevent_bufferevent/server.cpp)

target_link_libraries(BolSocket2 event)
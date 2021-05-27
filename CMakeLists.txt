cmake_minimum_required(VERSION 3.10)
project(BolSocket)

set(CMAKE_CXX_STANDARD 14)

add_executable(BolSocket Multi-process_server.cpp wrap.h wrap.cpp Multi-IO-Switching_server.cpp test.cpp Multi-IO-Epoll_server.cpp)
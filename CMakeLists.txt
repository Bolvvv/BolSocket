cmake_minimum_required(VERSION 3.10)
project(BolSocket)
set(CMAKE_CXX_STANDARD 14)

set(LINK_DIR /usr/local/lib)
set(INC_DIR /usr/local/include)

include_directories(${INC_DIR})
link_directories(${LINK_DIR})

add_executable(BolSocket main.cpp)

target_link_libraries(BolSocket event)
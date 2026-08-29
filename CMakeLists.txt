cmake_minimum_required(VERSION 3.15)
project(Screen-Jumper)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(raylib REQUIRED)
find_package(Threads REQUIRED)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE raylib)

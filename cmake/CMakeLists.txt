cmake_minimum_required(VERSION 3.10)

project(example)

set(CMAKE_CXX_STANDARD 17)

# 添加 crow.h 路径
include_directories("/home/user1/Crow/include" "/home/user1/asio/asio/include")

add_executable(main main.cpp)

# 链接 pthread 库
target_link_libraries(main pthread)

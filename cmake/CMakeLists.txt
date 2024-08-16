cmake_minimum_required(VERSION 3.10)

project(example)

set(CMAKE_CXX_STANDARD 17)

# add crow.h 
include_directories("/home/user1/Crow/include" "/home/user1/asio/asio/include")

add_executable(main main.cpp)

# link to pthread 
target_link_libraries(main pthread)

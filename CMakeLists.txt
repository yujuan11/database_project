cmake_minimum_required(VERSION 3.10)

project(db_project)

set(CMAKE_CXX_STANDARD 14)


find_package(mongocxx REQUIRED)
find_package(bsoncxx REQUIRED)


add_executable(db_project scr/main.cpp)

# add crow.h route
include_directories("/home/user1/Crow/include" "/home/user1/asio/asio/include")

# Include directories for bsoncxx and mongocxx
target_include_directories(db_project PRIVATE /usr/local/include/bsoncxx/v_noabi /usr/local/include/mongocxx/v_noabi /usr/local/include/bsoncxx/v_noabi/bsoncxx/third_party/mnmlstc)


# Link the MongoDB C++ driver libraries and specify the library directories
target_link_libraries(db_project PRIVATE /usr/local/lib64/libbsoncxx.so /usr/local/lib64/libmongocxx.so )


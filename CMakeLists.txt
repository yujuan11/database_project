cmake_minimum_required(VERSION 3.10)

project(my_project)

set(CMAKE_CXX_STANDARD 14)


find_package(mongocxx REQUIRED)
find_package(bsoncxx REQUIRED)


add_executable(my_project scr/main.cpp)


# Include directories for bsoncxx and mongocxx
target_include_directories(my_project PRIVATE /usr/local/include/bsoncxx/v_noabi /usr/local/include/mongocxx/v_noabi /usr/local/include/bsoncxx/v_noabi/bsoncxx/third_party/mnmlstc)


# Link the MongoDB C++ driver libraries and specify the library directories
target_link_libraries(my_project PRIVATE /usr/local/lib64/libbsoncxx.so /usr/local/lib64/libmongocxx.so )


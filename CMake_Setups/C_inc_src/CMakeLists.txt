cmake_minimum_required(VERSION 3.0.0)
project(c_cmake VERSION 0.1.0)

add_library(helper src/helper.c)
target_include_directories(helper PUBLIC include)

add_executable(c_cmake src/main.c)
target_link_libraries(c_cmake PRIVATE helper)

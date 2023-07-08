cmake_minimum_required(VERSION 3.0.0)
project(c_cmake VERSION 0.1.0)

add_subdirectory(helper)

list(APPEND EXTRA_LIBS helper)
list(APPEND EXTRA_INCLUDES ${PROJECT_SOURCE_DIR}/helper/include)

add_subdirectory(Application)

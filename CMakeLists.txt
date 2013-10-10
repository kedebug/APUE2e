cmake_minimum_required(VERSION 2.6)

project(apue CXX)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(lib)
add_subdirectory(ch03)

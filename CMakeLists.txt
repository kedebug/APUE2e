cmake_minimum_required(VERSION 2.6)

project(apue CXX)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(lib)
add_subdirectory(ch03)
add_subdirectory(ch08)
add_subdirectory(ch10)
add_subdirectory(ch11)
add_subdirectory(ch12)
add_subdirectory(ch20)

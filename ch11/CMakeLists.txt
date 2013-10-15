add_executable(cleanup cleanup.cc)
target_link_libraries(cleanup libapue pthread)

add_library(libmutex mutex.cc)
target_link_libraries(libmutex)
install(TARGETS libmutex DESTINATION ch11)

add_library(libhash hash.cc)
install(TARGETS libhash DESTINATION ch11)

add_library(libqueue queue.cc)
install(TARGETS libqueue DESTINATION ch11)

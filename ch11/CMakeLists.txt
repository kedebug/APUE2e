add_executable(cleanup cleanup.cc)
target_link_libraries(cleanup libapue pthread)

set(libch11
    mutex.cc
    hash.cc
    queue.cc
    condition.cc
)

add_library(ch11 ${libch11})
target_link_libraries(ch11 libapue)
install(TARGETS ch11 DESTINATION ch11)

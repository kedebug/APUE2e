add_executable(catchsig catchsig.cc)
target_link_libraries(catchsig libapue)

add_executable(reentrant reentrant.cc)
target_link_libraries(reentrant libapue)

add_executable(sleep2 sleep2.cc)
target_link_libraries(sleep2 libapue)

add_executable(read read.cc)
target_link_libraries(read libapue)

add_executable(critical critical.cc)
target_link_libraries(critical libapue)

add_executable(sleep sleep.cc)
target_link_libraries(sleep libapue)

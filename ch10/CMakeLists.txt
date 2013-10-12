add_executable(catchsig catchsig.cc)
target_link_libraries(catchsig libapue)

add_executable(reentrant reentrant.cc)
target_link_libraries(reentrant libapue)

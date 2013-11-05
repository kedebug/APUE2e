add_library(db db.cc)
target_link_libraries(db apue)
install(TARGET db DESTINATION ch20)

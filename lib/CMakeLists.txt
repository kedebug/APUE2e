set(base_lib
    error.cc
    prmask.cc
)

add_library(libapue ${base_lib})
target_link_libraries(libapue)

install(TARGETS libapue DESTINATION lib)

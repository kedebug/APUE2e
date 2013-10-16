set(libch12
    getenv.cc
    getenv1.cc
)

add_library(ch12 ${libch12})
install(TARGETS ch12 DESTINATION ch12)

add_executable(signal signal.cc)
target_link_libraries(signal libapue pthread)

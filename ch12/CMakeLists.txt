set(libch12
    getenv.cc
    getenv1.cc
)

add_library(ch12 ${libch12})
install(TARGETS ch12 DESTINATION ch12)

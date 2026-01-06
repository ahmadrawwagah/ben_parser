cmake_minimum_required(VERSION 3.16)
project(ben_parser)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
    main.cpp
    src/bencode.cpp
)
add_executable(ben_parser ${SOURCES})

target_compile_options(ben_parser PUBLIC -std=c++20 -Wall -Wextra -oFast)

target_include_directories(ben_parser PUBLIC include/)

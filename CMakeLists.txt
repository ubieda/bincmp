cmake_minimum_required(VERSION 3.20.0)
add_executable(bincmp)
project(bincmp)

target_include_directories(bincmp PRIVATE
    src
)

target_sources(bincmp PRIVATE
    src/main.c
)

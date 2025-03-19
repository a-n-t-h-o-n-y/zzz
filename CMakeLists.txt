cmake_minimum_required(VERSION 3.14)
Project(ZZZ LANGUAGES CXX)

add_library(zzz INTERFACE)

target_include_directories(zzz
    INTERFACE
        include/
)

add_subdirectory(test)
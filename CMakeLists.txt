cmake_minimum_required(VERSION 3.14)
Project(ZZZ LANGUAGES CXX)

add_library(zzz INTERFACE)

target_include_directories(zzz
    INTERFACE
        include/
)

target_compile_features(zzz
    INTERFACE
        cxx_std_17
)

target_compile_options(zzz
    INTERFACE
        -Wall
        -Wextra
        -Wpedantic
)

add_subdirectory(test)

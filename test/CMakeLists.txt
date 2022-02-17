cmake_minimum_required(VERSION 3.14)

add_executable(zzz-test
    test.main.cpp
)

target_link_libraries(zzz-test
    PUBLIC
        zzz
)

target_compile_features(zzz-test
    PUBLIC
        cxx_std_17
)

target_compile_options(zzz-test
    PUBLIC
        -Wall
        -Wextra
        -Wpedantic
)

cmake_minimum_required(VERSION 3.14)

add_executable(test-zzz
    test.main.cpp
)

target_link_libraries(test-zzz
    PRIVATE
        zzz
)

target_compile_features(test-zzz
    PRIVATE
        cxx_std_17
)

target_compile_options(test-zzz
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

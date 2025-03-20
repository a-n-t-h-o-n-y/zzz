cmake_minimum_required(VERSION 3.14)

add_executable(test-zzz
    container.test.cpp
    coro.test.cpp
    io.test.cpp
    string.test.cpp
    tuple.test.cpp
)

message(STATUS "AddressSanitizer Enabled")
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(test-zzz PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(test-zzz PRIVATE -fsanitize=address)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(test-zzz PRIVATE /fsanitize=address)
    target_link_options(test-zzz PRIVATE /INCREMENTAL:NO /DEBUG /fsanitize=address)
endif()

target_link_libraries(test-zzz
    PRIVATE
        zzz
)

target_compile_features(test-zzz
    PRIVATE
        cxx_std_20
)

target_compile_options(test-zzz
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

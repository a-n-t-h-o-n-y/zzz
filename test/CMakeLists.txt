# TESTS
add_executable(zzz.tests.unit EXCLUDE_FROM_ALL
    container.test.cpp
    coro.test.cpp
    io.test.cpp
    string.test.cpp
    tuple.test.cpp
)

target_link_libraries(zzz.tests.unit
    PRIVATE
        zzz
)

target_compile_options(zzz.tests.unit
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

message(STATUS "AddressSanitizer Enabled for zzz.tests.unit")
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(zzz.tests.unit
        PRIVATE
            -fsanitize=address
            -fno-omit-frame-pointer
    )
    target_link_options(zzz.tests.unit
        PRIVATE
            -fsanitize=address
    )
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(zzz.tests.unit
        PRIVATE
            /fsanitize=address
    )
    target_link_options(zzz.tests.unit
        PRIVATE
            /INCREMENTAL:NO
            /DEBUG
            /fsanitize=address
    )
endif()
cmake_minimum_required(VERSION 3.14)
Project(ZZZ LANGUAGES CXX)

add_library(zzz INTERFACE
    include/zzz/char_traits.hpp
    include/zzz/container.hpp
    include/zzz/coro.hpp
    include/zzz/io.hpp
    include/zzz/overload.hpp
    include/zzz/string.hpp
    include/zzz/test.hpp
    include/zzz/tuple.hpp
)

target_compile_features(zzz
    INTERFACE
        cxx_std_20
)

target_include_directories(zzz
    INTERFACE
        include/
)

add_subdirectory(test)
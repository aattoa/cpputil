cpputil_library(mem)

target_sources(cpputil_mem
    PRIVATE cpputil/mem/stable_string_pool.cpp
    PRIVATE cpputil/mem/stable_string_pool.hpp
    PRIVATE cpputil/mem/unstable_string_pool.cpp
    PRIVATE cpputil/mem/unstable_string_pool.hpp)

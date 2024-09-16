cpputil_library(fn)

target_sources(cpputil_fn
    PRIVATE cpputil/function_ref.cpp
    PUBLIC  cpputil/function_ref.hpp)

cmake_minimum_required(VERSION 3.24)
project(cpputil CXX)

set(CMAKE_CXX_STANDARD            23)
set(CMAKE_CXX_STANDARD_REQUIRED   ON)
set(CMAKE_CXX_EXTENSIONS          OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(${PROJECT_NAME} STATIC)
target_sources(${PROJECT_NAME}
    PRIVATE src/cpputil/io.cpp
    PRIVATE src/cpputil/io.hpp
    PRIVATE src/cpputil/util.cpp
    PRIVATE src/cpputil/util.hpp)
target_include_directories(cpputil PUBLIC src)

if (MSVC)
    target_compile_options(cpputil PRIVATE "/W4")
else ()
    target_compile_options(cpputil PRIVATE "-Wall" "-Wextra" "-Wpedantic")
endif ()

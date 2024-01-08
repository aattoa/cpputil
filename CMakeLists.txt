cmake_minimum_required(VERSION 3.24)
project(cpputil CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CPPUTIL_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/${PROJECT_NAME})

add_library(${PROJECT_NAME} STATIC)
target_sources(${PROJECT_NAME}
    PRIVATE ${CPPUTIL_SOURCE_DIR}/io.cpp
    PRIVATE ${CPPUTIL_SOURCE_DIR}/io.hpp
    PRIVATE ${CPPUTIL_SOURCE_DIR}/util.cpp
    PRIVATE ${CPPUTIL_SOURCE_DIR}/util.hpp)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE "/W4")
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE "-Wall" "-Wextra" "-Wpedantic")
endif ()

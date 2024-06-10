cpputil_library(input)

target_sources(cpputil_input
    PRIVATE cpputil/input.cpp
    PRIVATE cpputil/input.hpp)

unset(LIBREADLINE CACHE)
find_library(LIBREADLINE readline)

if (LIBREADLINE)
    target_link_libraries(cpputil_input PRIVATE ${LIBREADLINE})
else ()
    target_link_libraries(cpputil_input PRIVATE cpputil::io)
endif ()

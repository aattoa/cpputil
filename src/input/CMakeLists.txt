cpputil_library(input)

target_sources(cpputil_input
    PRIVATE cpputil/input.cpp
    PRIVATE cpputil/input.hpp)

if (READLINE)
    target_link_libraries(cpputil_input PRIVATE ${READLINE})
else ()
    target_link_libraries(cpputil_input PRIVATE cpputil::io)
endif ()

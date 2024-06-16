cpputil_library(json)

target_sources(cpputil_json
    PRIVATE cpputil/json.cpp
    PRIVATE cpputil/json.hpp
    PRIVATE cpputil/json/decode.cpp
    PRIVATE cpputil/json/decode.hpp
    PRIVATE cpputil/json/encode.hpp
    PRIVATE cpputil/json/format.hpp)

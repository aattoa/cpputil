include(FetchContent)
FetchContent_Declare(cppunittest
    GIT_REPOSITORY https://github.com/aattoa/cppunittest.git
    GIT_TAG        12e4bf4c101ae7c90be2cd68055d495d2b76a653)
FetchContent_MakeAvailable(cppunittest)
include(cppunittest)
add_subdirectory(json)
add_subdirectory(mem)

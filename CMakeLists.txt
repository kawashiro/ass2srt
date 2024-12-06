cmake_minimum_required(VERSION 3.25.0)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(
    ass2srt
    VERSION 0.0.1
    DESCRIPTION "Convert ASS subtitles toi SRT format easily keeping readability as much as possible"
    HOMEPAGE_URL "https://example.com/"
)

set(
    ASS2SRT_SOURCES
    src/ass.cpp
    src/ass/parserstate.cpp
    src/ass/section.cpp
    src/argparser.cpp
    src/somemodule.cpp
    src/strutils.cpp
)
set(
    ASS2SRT_TEST_SOURCES
    tests/ass_test.cpp
    tests/argparser_test.cpp
    tests/somemodule_test.cpp
)

add_executable(ass2srt ${ASS2SRT_SOURCES} src/main.cpp)
configure_file(src/config.h.in src/config.h)
target_include_directories(ass2srt PRIVATE "${PROJECT_BINARY_DIR}/src")
add_compile_options(-Wall -Wextra -Wpedantic)
string(APPEND CMAKE_CXX_FLAGS_DEBUG " -O0 -ggdb")

add_executable(ass2srt_test ${ASS2SRT_SOURCES} ${ASS2SRT_TEST_SOURCES})
target_include_directories(ass2srt_test PRIVATE src/)
target_link_libraries(ass2srt_test gtest gtest_main)

include(GoogleTest)
gtest_discover_tests(ass2srt_test)

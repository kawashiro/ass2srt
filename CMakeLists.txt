cmake_minimum_required(VERSION 3.25.0)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
    ass2srt
    VERSION 1.2.2
    DESCRIPTION "Convert ASS subtitles toi SRT format easily keeping readability as much as possible"
    HOMEPAGE_URL "https://github.com/kawashiro/ass2srt"
)

if (NOT CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX /usr/local/bin)
endif()
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

configure_file(src/config.h.in src/config.h)

set(
    ASS2SRT_SOURCES
    src/ass.cpp
    src/ass/field.cpp
    src/ass/line.cpp
    src/ass/parser.cpp
    src/ass/section.cpp
    src/ass/vpos.cpp
    src/argparser.cpp
    src/io.cpp
    src/merge.cpp
    src/srt.cpp
    src/strutils.cpp
    src/subline.cpp
)
set(
    ASS2SRT_TEST_SOURCES
    tests/ass_field_test.cpp
    tests/ass_line_test.cpp
    tests/ass_section_test.cpp
    tests/ass_vpos_test.cpp
    tests/ass_test.cpp
    tests/argparser_test.cpp
    tests/mathutils_test.cpp
    tests/merge_test.cpp
    tests/srt_test.cpp
    tests/statemachine_test.cpp
    tests/strutils_test.cpp
    tests/subline_test.cpp
)

set(
    ASS2SRT_UI_SOURCES
    src/ui/mainwindow.ui
    src/ui/mainwindow.cpp
    src/mainui.cpp
)

if (ASS2SRT_BUILD_UI)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    find_package(Qt6 REQUIRED COMPONENTS Widgets)
    qt_standard_project_setup()

    if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        enable_language("RC")
        set (WIN32_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/res/icon.rc)
    endif()

    qt_add_executable(ass2srt-gui ${ASS2SRT_SOURCES} ${ASS2SRT_UI_SOURCES} ${WIN32_RESOURCES})
    target_include_directories(ass2srt-gui PRIVATE "${PROJECT_BINARY_DIR}/src")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--whole-archive -lbz2 -lbrotlidec -lbrotlicommon -Wl,--no-whole-archive")
    target_link_libraries(ass2srt-gui PRIVATE freetype png z Qt6::Widgets)
    set_target_properties(ass2srt-gui PROPERTIES WIN32_EXECUTABLE ON)
    install(TARGETS ass2srt-gui DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
endif()

add_executable(ass2srt ${ASS2SRT_SOURCES} src/main.cpp)
target_include_directories(ass2srt PRIVATE "${PROJECT_BINARY_DIR}/src")
add_compile_options(-Wall -Wextra -Wpedantic)
string(APPEND CMAKE_CXX_FLAGS_DEBUG " -O0 -ggdb --coverage")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_executable(ass2srt_test ${ASS2SRT_SOURCES} ${ASS2SRT_TEST_SOURCES})
    target_include_directories(ass2srt_test PRIVATE src/)
    target_link_libraries(ass2srt_test gtest gtest_main)

    include(GoogleTest)
    gtest_discover_tests(ass2srt_test)
endif()

install(TARGETS ass2srt DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")

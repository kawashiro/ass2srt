#include <gtest/gtest.h>
#include <list>
#include <string>
#include "strutils.hpp"

using namespace ass2srt;

TEST(strutils, test_trim) {
    std::string value = "   hello   ";
    strutils::trim(value);
    ASSERT_EQ(value, "hello");
}

TEST(strutils, test_replace_all) {
    std::string value = "_hello_world_";
    strutils::replace_all(value, "_", "!!");
    ASSERT_EQ(value, "!!hello!!world!!");
}

TEST(strutils, test_split) {
    std::string value = "_hello_world_";
    auto res = strutils::split(value, '_');
    std::list<std::string> expected {"", "hello", "world", ""};
    ASSERT_EQ(res, expected);
}

TEST(strutils, test_format) {
    auto res = strutils::format("Hello, %s! (%d)", "world", 42);
    ASSERT_EQ(res, "Hello, world! (42)");
}

#include <gtest/gtest.h>
#include <list>
#include <string>
#include "strutils.hpp"
#include "subline.hpp"

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

TEST(strutils, test_subtitles_to_string) {
    const subtitles_t sub_struct {
        {
            140,
            4600,
            {
                {
                    0.5,
                    "This is ",
                },
                {
                    0.9,
                    "Some complex string,",
                },
                {
                    0.2,
                    " yo!",
                },
            },
        },
        {
            11280,
            12500,
            {
                {
                    0.0,
                    "And this is a simple one...",
                },
            },
        },
    };
    const std::string sub_str(
        "{\n"
        "    {\n"
        "        140,\n"
        "        4600,\n"
        "        {\n"
        "            {\n"
        "                0.500000,\n"
        "                \"This is \",\n"
        "            },\n"
        "            {\n"
        "                0.900000,\n"
        "                \"Some complex string,\",\n"
        "            },\n"
        "            {\n"
        "                0.200000,\n"
        "                \" yo!\",\n"
        "            },\n"
        "        },\n"
        "    },\n"
        "    {\n"
        "        11280,\n"
        "        12500,\n"
        "        {\n"
        "            {\n"
        "                0.000000,\n"
        "                \"And this is a simple one...\",\n"
        "            },\n"
        "        },\n"
        "    },\n"
        "}"
    );

    auto result = strutils::subtitles_to_string(sub_struct);
    ASSERT_EQ(result, sub_str) << result;
}

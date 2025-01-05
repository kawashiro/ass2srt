#include <sstream>
#include <gtest/gtest.h>
#include "srt.hpp"
#include "subline.hpp"

using namespace ass2srt;

static const subtitles_t input {
    {
        0,
        5000000,
        {
            {
                0.0,
                "Line #1 at the beginning",
            },
        },
    },
    {
        5014560,
        5020560,
        {
            {
                0.0,
                "Top line #1\nMiddle line #1\nBottom line #1\nLine #1 at the beginning",
            },
        },
    },
    {
        6000000,
        6000001,
        {
            {
                0.0,
                "Line #2 at the beginning",
            },
        },
    },
};

const char *expected = (
    "1\r\n"
    "00:00:00,000 --> 01:23:20,000\r\n"
    "Line #1 at the beginning\r\n"
    "\r\n"
    "2\r\n"
    "01:23:34,560 --> 01:23:40,560\r\n"
    "Top line #1\r\n"
    "Middle line #1\r\n"
    "Bottom line #1\r\n"
    "Line #1 at the beginning\r\n"
    "\r\n"
    "3\r\n"
    "01:40:00,000 --> 01:40:00,000\r\n"
    "Line #2 at the beginning\r\n"
);

TEST(srt, test_write_srt_stream) {
    std::ostringstream out;
    std::string expected_str(expected);

    srt::write_srt_stream(input, out);
    std::string result_str = out.str();

    ASSERT_EQ(result_str, expected_str);
}

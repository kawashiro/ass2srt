#include "merge.hpp"
#include "strutils.hpp"
#include "subline.hpp"
#include <gtest/gtest.h>

using namespace ass2srt;

static const subtitles_t input {
    {
        0,
        100,
        {
            {
                0.5,
                0,
                "",
            },
        },
    },
    {
        10,
        20,
        {
            {
                1.0,
                0,
                "Top line #1",
            },
            {
                0.1,
                0,
                "Bottom line #1",
            },
            {
                0.5,
                0,
                "Middle line #1",
            },
        },
    },
    {
        50,
        60,
        {
            {
                0.4,
                0,
                "Middle line #2",
            },
        },
    },
    {
        15,
        30,
        {
            {
                0.0,
                0,
                "Intersects #1 at the end",
            },
        },
    },
    {
        5,
        12,
        {
            {
                0.0,
                0,
                "Intersects #1 at the beginning",
            },
        },
    },
    {
        54,
        56,
        {
            {
                0.0,
                0,
                "Intersects #2 in the middle",
            },
        },
    },
    {
        1,
        2,
        {
            {
                0.9,
                0,
                "Top line #0",
            },
        },
    },
    {
        75,
        76,
        {
            {
                0.0,
                0,
                "Short line",
            },
        },
    },
    {
        59,
        80,
        {
            {
                0.0,
                0,
                "Long line on top of short line",
            },
        },
    },
    {
        13,
        14,
        {
            {
                0.0,
                0,
                "Should keep all old lines",
            },
        },
    },
    {
        77,
        80,
        {
            {
                0.0,
                0,
                "Long line on top of short line",
            },
        },
    },
    {
        81,
        82,
        {
            {
                0.0,
                2,
                "This should be the second",
            },
            {
                0.0,
                1,
                "This should be the first",
            },
        },
    },
};

static const subtitles_t expected {
    {
        1,
        2,
        {
            {
                0.0,
                0,
                "Top line #0",
            },
        },
    },
    {
        5,
        10,
        {
            {
                0.0,
                0,
                "Intersects #1 at the beginning",
            },
        },
    },
    {
        10,
        12,
        {
            {
                0.0,
                0,
                "Top line #1\nMiddle line #1\nBottom line #1\nIntersects #1 at the beginning",
            },
        },
    },
    {
        12,
        13,
        {
            {
                0.0,
                0,
                "Top line #1\nMiddle line #1\nBottom line #1",
            },
        },
    },
    {
        13,
        14,
        {
            {
                0.0,
                0,
                "Top line #1\nMiddle line #1\nBottom line #1\nShould keep all old lines",
            },
        },
    },
    {
        14,
        15,
        {
            {
                0.0,
                0,
                "Top line #1\nMiddle line #1\nBottom line #1",
            },
        },
    },
    {
        15,
        20,
        {
            {
                0.0,
                0,
                "Top line #1\nMiddle line #1\nBottom line #1\nIntersects #1 at the end",
            },
        },
    },
    {
        20,
        30,
        {
            {
                0.0,
                0,
                "Intersects #1 at the end",
            },
        },
    },
    {
        50,
        54,
        {
            {
                0.0,
                0,
                "Middle line #2",
            },
        },
    },
    {
        54,
        56,
        {
            {
                0.0,
                0,
                "Middle line #2\nIntersects #2 in the middle",
            },
        },
    },
    {
        56,
        59,
        {
            {
                0.0,
                0,
                "Middle line #2",
            },
        },
    },
    {
        59,
        60,
        {
            {
                0.0,
                0,
                "Middle line #2\nLong line on top of short line",
            },
        },
    },
    {
        60,
        75,
        {
            {
                0.0,
                0,
                "Long line on top of short line",
            },
        },
    },
    {
        75,
        76,
        {
            {
                0.0,
                0,
                "Long line on top of short lineShort line",
            },
        },
    },
    {
        76,
        77,
        {
            {
                0.0,
                0,
                "Long line on top of short line",
            },
        },
    },
    {
        77,
        80,
        {
            {
                0.0,
                0,
                "Long line on top of short line",
            },
        },
    },
    {
        81,
        82,
        {
            {
                0.0,
                0,
                "This should be the firstThis should be the second",
            },
        },
    },
};

TEST(Merge, TestMergeSubtitlesParts)
{
    auto result = merge::merge_subtitles_parts(input);
    ASSERT_EQ(result, expected) << "Subtitles did not match. Actual result: " << strutils::subtitles_to_string(result);
}

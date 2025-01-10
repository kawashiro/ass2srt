#include "ass/section.hpp"
#include <gtest/gtest.h>
#include <stdexcept>

using namespace ass2srt;

TEST(AssSection, TestParse)
{
    auto res = ass::section::parse("[Events]");
    ASSERT_EQ(res, ass::section::EVENTS);
}

TEST(AssSection, TestParseUnknown)
{
    auto res = ass::section::parse("[Unsupported]");
    ASSERT_EQ(res, ass::section::UNKNOWN);
}

TEST(AssSection, TestParseInvalid)
{
    ASSERT_THROW(ass::section::parse("whatever"), std::invalid_argument);
}

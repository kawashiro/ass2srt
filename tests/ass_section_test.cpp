#include <stdexcept>
#include <gtest/gtest.h>
#include "ass/section.hpp"

using namespace ass2srt;

TEST(ass_section, test_parse) {
    auto res = ass::section::parse("[Events]");
    ASSERT_EQ(res, ass::section::EVENTS);
}

TEST(ass_section, test_parse_unknown) {
    auto res = ass::section::parse("[Unsupported]");
    ASSERT_EQ(res, ass::section::UNKNOWN);
}

TEST(ass_section, test_parse_invalid) {
    ASSERT_THROW(ass::section::parse("whatever"), std::invalid_argument);
}

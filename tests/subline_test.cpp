#include <gtest/gtest.h>
#include "subline.hpp"

using namespace ass2srt;

TEST(test_subline, test_eq) {
    subline line1 { 100, 200, { { 0.3, "Text1" } } };
    subline line2 { 100, 200, { { 0.3, "Text1" } } };
    subline line3 { 100, 200, { { 0.3, "Text2" } } };

    ASSERT_TRUE(line1 == line2);
    ASSERT_FALSE(line1 == line3);
}

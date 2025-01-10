#include "subline.hpp"
#include <gtest/gtest.h>

using namespace ass2srt;

TEST(TestSubline, TestEq)
{
    const subline line1 { 100, 200, { { 0.3, 0, "Text1" } } };
    const subline line2 { 100, 200, { { 0.3, 0, "Text1" } } };
    const subline line3 { 100, 200, { { 0.3, 0, "Text2" } } };

    ASSERT_TRUE(line1 == line2);
    ASSERT_FALSE(line1 == line3);
}

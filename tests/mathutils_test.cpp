#include "mathutils.hpp"
#include <cstdint>
#include <gtest/gtest.h>

using namespace ass2srt;

TEST(MathUtils, TestIsClose)
{
    const uint32_t val1_bin = 0x20000001;
    const uint32_t val2_bin = 0x20000002;

    const float val1_f = *(float*)&val1_bin;
    const float val2_f = *(float*)&val2_bin;
    ASSERT_TRUE(mathutils::is_close(val1_f, val2_f));

    ASSERT_FALSE(mathutils::is_close(3.5F, 19.2F));
}

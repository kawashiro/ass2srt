#include <gtest/gtest.h>
#include "somemodule.hpp"

TEST(somemodule, get42) {
    int res = ass2srt::get42();
    EXPECT_EQ(res, 42);
}

#include "ass/field.hpp"
#include "ass/vpos.hpp"
#include <gtest/gtest.h>
#include <stdexcept>

using namespace ass2srt;

TEST(AssVpos, TestCalcByExplicitPos)
{
    const ass::field::styles_spec_t line_style;
    const ass::field::styles_spec_t inline_style(ALIGN(ALIGN_BOTTOM, ALIGN_CENTER), -1, 30, false);
    auto res = ass::vpos::calculate_vpos(100, 0, line_style, inline_style);
    ASSERT_FLOAT_EQ(res, 0.7);
}

TEST(AssVpos, TestCalcHasLineAlignBottom)
{
    const ass::field::styles_spec_t line_style(ALIGN(ALIGN_BOTTOM, ALIGN_CENTER), 20, -1, false);
    const ass::field::styles_spec_t inline_style;
    auto res = ass::vpos::calculate_vpos(100, 40, line_style, inline_style);
    ASSERT_FLOAT_EQ(res, 0.4);
}

TEST(AssVpos, TestCalcHasLineStyleAlignBottom)
{
    const ass::field::styles_spec_t line_style(ALIGN(ALIGN_BOTTOM, ALIGN_CENTER), 20, -1, false);
    const ass::field::styles_spec_t inline_style;
    auto res = ass::vpos::calculate_vpos(100, -1, line_style, inline_style);
    ASSERT_FLOAT_EQ(res, 0.2);
}

TEST(AssVpos, TestCalcHasInlineStyleAlignBottom)
{
    const ass::field::styles_spec_t line_style(ALIGN(ALIGN_BOTTOM, ALIGN_CENTER), 20, -1, false);
    const ass::field::styles_spec_t inline_style(ALIGN(ALIGN_BOTTOM, ALIGN_CENTER), 10, -1, false);
    auto res = ass::vpos::calculate_vpos(100, 0, line_style, inline_style);
    ASSERT_FLOAT_EQ(res, 0.1);
}

TEST(AssVpos, TestCalcAlignTop)
{
    const ass::field::styles_spec_t line_style;
    const ass::field::styles_spec_t inline_style(ALIGN(ALIGN_TOP, ALIGN_RIGHT), 10, -1, false);
    auto res = ass::vpos::calculate_vpos(100, 0, line_style, inline_style);
    ASSERT_FLOAT_EQ(res, 0.9);
}

TEST(AssVpos, TestCalcAlignMiddle)
{
    const ass::field::styles_spec_t line_style;
    const ass::field::styles_spec_t inline_style(ALIGN(ALIGN_MIDDLE, ALIGN_RIGHT), 10, -1, false);
    auto res = ass::vpos::calculate_vpos(100, 0, line_style, inline_style);
    ASSERT_FLOAT_EQ(res, 0.5);
}

TEST(AssVpos, TestCalcBadAlignment)
{
    const ass::field::styles_spec_t line_style;
    const ass::field::styles_spec_t inline_style(42, -1, -1, false);
    ASSERT_THROW(ass::vpos::calculate_vpos(100, 0, line_style, inline_style), std::invalid_argument);
}

TEST(AssVpos, TestCalcNoVsizeAlignBottom)
{
    const ass::field::styles_spec_t line_style;
    const ass::field::styles_spec_t inline_style(ALIGN(ALIGN_BOTTOM, ALIGN_CENTER), -1, -1, false);
    auto res = ass::vpos::calculate_vpos(0, 0, line_style, inline_style);
    ASSERT_FLOAT_EQ(res, 0.0);
}

TEST(AssVpos, TestCalcNoVsizeAlignMiddle)
{
    const ass::field::styles_spec_t line_style;
    const ass::field::styles_spec_t inline_style(ALIGN(ALIGN_MIDDLE, ALIGN_CENTER), -1, -1, false);
    auto res = ass::vpos::calculate_vpos(0, 0, line_style, inline_style);
    ASSERT_FLOAT_EQ(res, 0.5);
}

TEST(AssVpos, TestCalcNoVsizeAlignTop)
{
    const ass::field::styles_spec_t line_style;
    const ass::field::styles_spec_t inline_style(ALIGN(ALIGN_TOP, ALIGN_CENTER), -1, -1, false);
    auto res = ass::vpos::calculate_vpos(0, 0, line_style, inline_style);
    ASSERT_FLOAT_EQ(res, 1.0);
}

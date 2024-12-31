#include <stdexcept>
#include <gtest/gtest.h>
#include "ass/field.hpp"
#include "ass/vpos.hpp"

using namespace ass2srt;

TEST(ass_vpos, test_calc_by_explicit_pos) {
    ass::field::styles_spec_t line_style;
    ass::field::styles_spec_t inline_style(0, -1, 30);
    auto res = ass::vpos::calculate_vpos(100, 0, line_style, inline_style);
    ASSERT_FLOAT_EQ(res, 0.3);
}

TEST(ass_vpos, test_calc_has_line_align_bottom) {
    ass::field::styles_spec_t line_style(ALIGN(ALIGN_BOTTOM, ALIGN_CENTER), 20, -1);
    ass::field::styles_spec_t inline_style;
    auto res = ass::vpos::calculate_vpos(100, 40, line_style, inline_style);
    ASSERT_FLOAT_EQ(res, 0.4);
}

TEST(ass_vpos, test_calc_has_line_style_align_bottom) {
    ass::field::styles_spec_t line_style(ALIGN(ALIGN_BOTTOM, ALIGN_CENTER), 20, -1);
    ass::field::styles_spec_t inline_style;
    auto res = ass::vpos::calculate_vpos(100, -1, line_style, inline_style);
    ASSERT_FLOAT_EQ(res, 0.2);
}

TEST(ass_vpos, test_calc_has_inline_style_align_bottom) {
    ass::field::styles_spec_t line_style(ALIGN(ALIGN_BOTTOM, ALIGN_CENTER), 20, -1);
    ass::field::styles_spec_t inline_style(ALIGN(ALIGN_BOTTOM, ALIGN_CENTER), 10, -1);
    auto res = ass::vpos::calculate_vpos(100, 0, line_style, inline_style);
    ASSERT_FLOAT_EQ(res, 0.1);
}

TEST(ass_vpos, test_calc_align_top) {
    ass::field::styles_spec_t line_style;
    ass::field::styles_spec_t inline_style(ALIGN(ALIGN_TOP, ALIGN_RIGHT), 10, -1);
    auto res = ass::vpos::calculate_vpos(100, 0, line_style, inline_style);
    ASSERT_FLOAT_EQ(res, 0.9);
}

TEST(ass_vpos, test_calc_align_middle) {
    ass::field::styles_spec_t line_style;
    ass::field::styles_spec_t inline_style(ALIGN(ALIGN_MIDDLE, ALIGN_RIGHT), 10, -1);
    auto res = ass::vpos::calculate_vpos(100, 0, line_style, inline_style);
    ASSERT_FLOAT_EQ(res, 0.5);
}

TEST(ass_vpos, test_calc_bad_alignment) {
    ass::field::styles_spec_t line_style;
    ass::field::styles_spec_t inline_style(42, -1, -1);
    ASSERT_THROW(ass::vpos::calculate_vpos(100, 0, line_style, inline_style), std::invalid_argument);
}

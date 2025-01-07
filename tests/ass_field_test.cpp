#include <stdexcept>
#include <gtest/gtest.h>
#include "ass/field.hpp"

using namespace ass2srt;

TEST(ass_field, test_parse_good_type) {
    auto res = ass::field::parse_type("MarginV");
    ASSERT_EQ(res, ass::field::MARGIN_V);
}

TEST(ass_field, test_parse_unknown_type) {
    auto res = ass::field::parse_type("Valera");
    ASSERT_EQ(res, ass::field::UNKNOWN);
}

TEST(ass_field, parse_time_millis) {
    auto res = ass::field::parse_time_millis("2:02:02.02");
    ASSERT_EQ(res, 7322020);
}

TEST(ass_field, parse_time_millis_bad_value) {
    ASSERT_THROW(ass::field::parse_time_millis("2:02~02.02"), std::invalid_argument);
}

TEST(ass_field, parse_inline_style_alignment) {
    auto res = ass::field::parse_inline_style("{\\a7}");
    ASSERT_EQ(res.alignment, ALIGN(ALIGN_TOP, ALIGN_LEFT));
}

TEST(ass_field, parse_inline_style_alignment_bad_value) {
    ASSERT_THROW(ass::field::parse_inline_style("{\\a42}"), std::invalid_argument);
}

TEST(ass_field, parse_inline_style_pos) {
    auto res = ass::field::parse_inline_style("{\\pos(42,43)}");
    ASSERT_EQ(res.explicit_y_pos, 43);
}

TEST(ass_field, parse_inline_style_pos_bad_value) {
    ASSERT_THROW(ass::field::parse_inline_style("{\\pos(42)}"), std::invalid_argument);
}

TEST(ass_field, parse_inline_style_move) {
    auto res = ass::field::parse_inline_style("{\\move(42,43,44,45)}");
    ASSERT_EQ(res.explicit_y_pos, 44);
}

TEST(ass_field, parse_inline_style_move_bad_value) {
    ASSERT_THROW(ass::field::parse_inline_style("{\\move(42)}"), std::invalid_argument);
}

TEST(ass_field, parse_inline_style_combined) {
    auto res = ass::field::parse_inline_style("{\\move(42,43,44,45)\\sOmeCMD\\broken(11,22,\\a7}");
    ASSERT_EQ(res.alignment, ALIGN(ALIGN_TOP, ALIGN_LEFT));
    ASSERT_EQ(res.explicit_y_pos, 44);
}

TEST(ass_field, parse_inline_style_drawing_on) {
    auto res = ass::field::parse_inline_style("{\\p1\\pos(643,655)}");
    ASSERT_TRUE(res.is_drawing);
}

TEST(ass_field, parse_inline_style_drawing_off) {
    auto res = ass::field::parse_inline_style("{\\p0\\pos(643,655)}");
    ASSERT_FALSE(res.is_drawing);
}

TEST(ass_field, parse_plain_text) {
    auto res = ass::field::parse_plain_text("This is\\Ntext!");
    ASSERT_EQ(res, "This is\ntext!");
}

TEST(ass_field, line_values_parser) {
    ass::field::LineValuesParser parser;
    
    parser.on<std::string>(ass::field::STYLE, [](std::string &val) { return new std::string(val); });
    parser.on<std::string>(ass::field::TEXT, [](std::string &val) { return new std::string(val); });

    parser.parse({ass::field::STYLE, ass::field::ALIGNMENT, ass::field::TEXT}, "SomeStyle,2,This,is,text!");

    ASSERT_EQ(parser.get<std::string>(ass::field::STYLE), "SomeStyle");
    ASSERT_EQ(parser.get<std::string>(ass::field::TEXT), "This,is,text!");
    ASSERT_THROW(parser.get<int>(ass::field::END), std::invalid_argument);
}

TEST(ass_styles_spec, init) {
    ass::field::styles_spec_t defalt_style, some_style2;
    ASSERT_EQ(defalt_style.alignment, 0);
    ASSERT_EQ(defalt_style.margin_v, -1);
    ASSERT_EQ(defalt_style.explicit_y_pos, -1);

    ass::field::styles_spec_t some_style(ALIGN_DEFAULT, 10, 20, false);
    ASSERT_EQ(some_style.alignment, ALIGN_DEFAULT);
    ASSERT_EQ(some_style.margin_v, 10);
    ASSERT_EQ(some_style.explicit_y_pos, 20);

    auto some_style_copy = some_style;
    ASSERT_EQ(some_style_copy.alignment, ALIGN_DEFAULT);
    ASSERT_EQ(some_style_copy.margin_v, 10);
    ASSERT_EQ(some_style_copy.explicit_y_pos, 20);

    some_style2 = some_style;
    ASSERT_EQ(some_style2.alignment, ALIGN_DEFAULT);
    ASSERT_EQ(some_style2.margin_v, 10);
    ASSERT_EQ(some_style2.explicit_y_pos, 20);
}
#include "ass/field.hpp"
#include <gtest/gtest.h>
#include <stdexcept>

using namespace ass2srt;

TEST(AssField, TestParseGoodType)
{
    auto res = ass::field::parse_type("MarginV");
    ASSERT_EQ(res, ass::field::MARGIN_V);
}

TEST(AssField, TestParseUnknownType)
{
    auto res = ass::field::parse_type("Valera");
    ASSERT_EQ(res, ass::field::UNKNOWN);
}

TEST(AssField, ParseTimeMillis)
{
    auto res = ass::field::parse_time_millis("2:02:02.02");
    ASSERT_EQ(res, 7322020);
}

TEST(AssField, ParseTimeMillisBadValue)
{
    ASSERT_THROW(ass::field::parse_time_millis("2:02~02.02"), std::invalid_argument);
}

TEST(AssField, ParseInlineStyleAlignment)
{
    auto res = ass::field::parse_inline_style("{\\a7}");
    ASSERT_EQ(res.alignment, ALIGN(ALIGN_TOP, ALIGN_LEFT));
}

TEST(AssField, ParseInlineStyleAlignmentBadValue)
{
    ASSERT_THROW(ass::field::parse_inline_style("{\\a42}"), std::invalid_argument);
}

TEST(AssField, ParseInlineStylePos)
{
    auto res = ass::field::parse_inline_style("{\\pos(42,43)}");
    ASSERT_EQ(res.explicit_y_pos, 43);
}

TEST(AssField, ParseInlineStylePosBadValue)
{
    ASSERT_THROW(ass::field::parse_inline_style("{\\pos(42)}"), std::invalid_argument);
}

TEST(AssField, ParseInlineStyleMove)
{
    auto res = ass::field::parse_inline_style("{\\move(42,43,44,45)}");
    ASSERT_EQ(res.explicit_y_pos, 44);
}

TEST(AssField, ParseInlineStyleMoveBadValue)
{
    ASSERT_THROW(ass::field::parse_inline_style("{\\move(42)}"), std::invalid_argument);
}

TEST(AssField, ParseInlineStyleCombined)
{
    auto res = ass::field::parse_inline_style(R"({\move(42,43,44,45)\sOmeCMD\broken(11,22,\a7})");
    ASSERT_EQ(res.alignment, ALIGN(ALIGN_TOP, ALIGN_LEFT));
    ASSERT_EQ(res.explicit_y_pos, 44);
}

TEST(AssField, ParseInlineStyleDrawingOn)
{
    auto res = ass::field::parse_inline_style("{\\p1\\pos(643,655)}");
    ASSERT_TRUE(res.is_drawing);
}

TEST(AssField, ParseInlineStyleDrawingOff)
{
    auto res = ass::field::parse_inline_style("{\\p0\\pos(643,655)}");
    ASSERT_FALSE(res.is_drawing);
}

TEST(AssField, ParsePlainText)
{
    auto res = ass::field::parse_plain_text("This is\\Ntext!");
    ASSERT_EQ(res, "This is\ntext!");
}

TEST(AssField, LineValuesParser)
{
    ass::field::LineValuesParser parser;

    parser.on<std::string>(ass::field::STYLE, [](std::string& val) { return new std::string(val); });
    parser.on<std::string>(ass::field::TEXT, [](std::string& val) { return new std::string(val); });

    parser.parse({ ass::field::STYLE, ass::field::ALIGNMENT, ass::field::TEXT }, "SomeStyle,2,This,is,text!");

    ASSERT_EQ(parser.get<std::string>(ass::field::STYLE), "SomeStyle");
    ASSERT_EQ(parser.get<std::string>(ass::field::TEXT), "This,is,text!");
    ASSERT_THROW(parser.get<int>(ass::field::END), std::invalid_argument);
}

TEST(AssStylesSpec, Init)
{
    const int margin_v = 10;
    const int explicit_y_pos = 20;

    ass::field::styles_spec_t defalt_style; // NOLINT
    ass::field::styles_spec_t some_style2; // NOLINT
    ASSERT_EQ(defalt_style.alignment, 0);
    ASSERT_EQ(defalt_style.margin_v, -1);
    ASSERT_EQ(defalt_style.explicit_y_pos, -1);

    const ass::field::styles_spec_t some_style(ALIGN_DEFAULT, margin_v, explicit_y_pos, false);
    ASSERT_EQ(some_style.alignment, ALIGN_DEFAULT);
    ASSERT_EQ(some_style.margin_v, margin_v);
    ASSERT_EQ(some_style.explicit_y_pos, explicit_y_pos);

    auto some_style_copy = some_style; // NOLINT
    ASSERT_EQ(some_style_copy.alignment, ALIGN_DEFAULT);
    ASSERT_EQ(some_style_copy.margin_v, margin_v);
    ASSERT_EQ(some_style_copy.explicit_y_pos, explicit_y_pos);

    some_style2 = some_style; // NOLINT
    ASSERT_EQ(some_style2.alignment, ALIGN_DEFAULT);
    ASSERT_EQ(some_style2.margin_v, margin_v);
    ASSERT_EQ(some_style2.explicit_y_pos, explicit_y_pos);
}

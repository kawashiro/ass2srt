#include "ass/field.hpp"
#include "ass/line.hpp"
#include <gtest/gtest.h>
#include <list>
#include <stdexcept>

using namespace ass2srt;

TEST(AssLine, TestParseGoodType)
{
    auto res = ass::line::parse_type("Format: A,B,C");
    ASSERT_EQ(res, ass::line::FORMAT);
}

TEST(AssLine, TestParseUnknownType)
{
    auto res = ass::line::parse_type("Valera: Whatever");
    ASSERT_EQ(res, ass::line::UNKNOWN);
}

TEST(AssLine, TestParseBadValueType)
{
    ASSERT_THROW(ass::line::parse_type("Who knows~"), std::invalid_argument);
}

TEST(AssLine, TestGetLineValue)
{
    auto value = ass::line::get_line_value("Format: A,B,C");
    ASSERT_EQ(value, "A,B,C");
}

TEST(AssLine, TestParseFormatDeclaration)
{
    auto res = ass::line::parse_format_declaration("Name, Start, End", {});
    const std::list<ass::field::FieldType> expected { ass::field::NAME, ass::field::START, ass::field::END };
    ASSERT_EQ(res, expected);
}

TEST(AssLine, TestParseFormatDeclarationMissingRequired)
{
    ASSERT_THROW(ass::line::parse_format_declaration("Name, End", { "Name", "Start", "End" }), std::invalid_argument);
}

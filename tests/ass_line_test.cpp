#include <list>
#include <stdexcept>
#include <gtest/gtest.h>
#include "ass/field.hpp"
#include "ass/line.hpp"

using namespace ass2srt;

TEST(ass_line, test_parse_good_type) {
    auto res = ass::line::parse_type("Format: A,B,C");
    ASSERT_EQ(res, ass::line::FORMAT);
}

TEST(ass_line, test_parse_unknown_type) {
    auto res = ass::line::parse_type("Valera: Whatever");
    ASSERT_EQ(res, ass::line::UNKNOWN);
}

TEST(ass_line, test_parse_bad_value_type) {
    ASSERT_THROW(ass::line::parse_type("Who knows~"), std::invalid_argument);
}

TEST(ass_line, test_get_line_value) {
    auto value = ass::line::get_line_value("Format: A,B,C");
    ASSERT_EQ(value, "A,B,C");
}

TEST(ass_line, test_parse_format_declaration) {
    auto res = ass::line::parse_format_declaration("Name, Start, End", {});
    std::list<ass::field::FieldType> expected {ass::field::NAME, ass::field::START, ass::field::END};
    ASSERT_EQ(res, expected);
}

TEST(ass_line, test_parse_format_declaration_missing_required) {
    ASSERT_THROW(ass::line::parse_format_declaration("Name, End", {"Name", "Start", "End"}), std::invalid_argument);
}

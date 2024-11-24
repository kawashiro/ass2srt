#include <set>
#include <string>
#include <unistd.h>
#include <gtest/gtest.h>
#include "argparser.hpp"

using namespace ass2srt;

class ParseArgsTest : public testing::Test {
    protected:

    void SetUp() override {
        optind = 1;
    }
};

TEST_F(ParseArgsTest, parse_args_normal) {
    int input_argc = 10;
    const char *input_argv[] = {"ass2srt", "-i", "/tmp/input.ass", "-o", "/tmp/output.srt", "-s", "style1,style2", "-e", "style3,style4", "-x", 0};

    std::set<std::string> expected_styles_scope {"style1", "style2"};
    std::set<std::string> expected_excluded_styles {"style3", "style4"};
    std::string expected_input_file = "/tmp/input.ass";
    std::string expected_output_file = "/tmp/output.srt";

    auto result = argparser::parse_args(input_argc, (char**)input_argv);
    EXPECT_EQ(result.input_file, expected_input_file);
    EXPECT_EQ(result.output_file, expected_output_file);
    EXPECT_EQ(result.styles_scope, expected_styles_scope);
    EXPECT_EQ(result.excluded_styles, expected_excluded_styles);
    EXPECT_TRUE(result.exclude_signs);
    EXPECT_TRUE(result.is_valid());
    EXPECT_FALSE(result.has_extra_opts);
}

TEST_F(ParseArgsTest, parse_args_extra_opts) {
    int input_argc = 6;
    const char *input_argv[] = {"ass2srt", "-i", "/tmp/input.ass", "-o", "/tmp/output.srt", "-z", 0};

    auto result = argparser::parse_args(input_argc, (char**)input_argv);
    EXPECT_TRUE(result.has_extra_opts);
    EXPECT_FALSE(result.is_valid());
}

TEST_F(ParseArgsTest, parse_args_missing_input) {
    int input_argc = 3;
    const char *input_argv[] = {"ass2srt", "-o", "/tmp/output.srt", 0};

    auto result = argparser::parse_args(input_argc, (char**)input_argv);
    EXPECT_FALSE(result.is_valid());
}

TEST_F(ParseArgsTest, parse_args_missing_output) {
    int input_argc = 3;
    const char *input_argv[] = {"ass2srt", "-i", "/tmp/input.srt", 0};

    auto result = argparser::parse_args(input_argc, (char**)input_argv);
    EXPECT_FALSE(result.is_valid());
}

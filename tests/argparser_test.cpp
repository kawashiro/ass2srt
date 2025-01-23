#include "argparser.hpp"
#include <gtest/gtest.h>
#include <set>
#include <string>
#include <unistd.h> // NOLINT

using namespace ass2srt;

class ParseArgsTest : public testing::Test {
protected:
    void SetUp() override
    {
        optind = 1; // NOLINT
    }
};

TEST_F(ParseArgsTest, ParseArgsNormal)
{
    const int input_argc = 10;
    const char* input_argv[] = { "ass2srt", "-i", "/tmp/input.ass", "-o", "/tmp/output.srt", "-s", "style1,style2", "-e", "style3,style4", "-x", 0 };

    const std::set<std::string> expected_styles_scope { "style1", "style2" };
    const std::set<std::string> expected_excluded_styles { "style3", "style4" };
    const std::string expected_input_file = "/tmp/input.ass";
    const std::string expected_output_file = "/tmp/output.srt";

    auto result = argparser::parse_args(input_argc, (char**)input_argv);
    EXPECT_EQ(result.input_file, expected_input_file);
    EXPECT_EQ(result.output_file, expected_output_file);
    EXPECT_EQ(result.styles_scope, expected_styles_scope);
    EXPECT_EQ(result.excluded_styles, expected_excluded_styles);
    EXPECT_TRUE(result.exclude_signs);
    EXPECT_TRUE(result.valid());
    EXPECT_FALSE(result.has_extra_opts);
}

TEST_F(ParseArgsTest, ParseArgsExtraOpts)
{
    const int input_argc = 6;
    const char* input_argv[] = { "ass2srt", "-i", "/tmp/input.ass", "-o", "/tmp/output.srt", "-z", 0 };

    auto result = argparser::parse_args(input_argc, (char**)input_argv);
    EXPECT_TRUE(result.has_extra_opts);
    EXPECT_FALSE(result.valid());
}

TEST_F(ParseArgsTest, ParseArgsMissingInput)
{
    const int input_argc = 3;
    const char* input_argv[] = { "ass2srt", "-o", "/tmp/output.srt", 0 };

    auto result = argparser::parse_args(input_argc, (char**)input_argv);
    EXPECT_FALSE(result.valid());
}

TEST_F(ParseArgsTest, ParseArgsMissingOutput)
{
    const int input_argc = 3;
    const char* input_argv[] = { "ass2srt", "-i", "/tmp/input.srt", 0 };

    auto result = argparser::parse_args(input_argc, (char**)input_argv);
    EXPECT_FALSE(result.valid());
}

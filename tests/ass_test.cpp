#include "ass.hpp"
#include "strutils.hpp"
#include "subline.hpp"
#include <cstring>
#include <gtest/gtest.h>
#include <span>
#include <spanstream>
#include <stdexcept>
#include <string>

using namespace ass2srt;

#define ASSERT_THROW_MSG(EXPR, EXC_CLS, MSG)                                                         \
    do {                                                                                             \
        try {                                                                                        \
            (EXPR);                                                                                  \
            FAIL() << "Exception " << #EXC_CLS << " was not thrown";                                 \
        } catch (const EXC_CLS& e) {                                                                 \
            ASSERT_EQ(std::string(e.what()), std::string(MSG)) << "Exception message did not match"; \
        } catch (...) {                                                                              \
            FAIL() << "Exception other than " << #EXC_CLS << " was thrown";                          \
        }                                                                                            \
    } while (0);

static const char* const good_file_content = ("\xEF\xBB\xBF[Script Info]\n"
                                              "; Script generated by Aegisub 2.1.7\n"
                                              "; http://www.aegisub.net\n"
                                              "Title: Default Aegisub file\n"
                                              "PlayResX: 1280\n"
                                              "PlayResY: 720\n"
                                              "\n"
                                              "[V4+ Styles]\n"
                                              "Format: Name, Fontname, Fontsize, PrimaryColour, SecondaryColour, OutlineColour, BackColour, Bold, Italic, Underline, StrikeOut, ScaleX, ScaleY, Spacing, Angle, BorderStyle, Outline, Shadow, Alignment, MarginL, MarginR, MarginV, Encoding\n"
                                              "Style: Default,a_FuturaRound,48,&H00FFEEEE,&H1EFFFFFF,&H00220000,&H00000000,-1,0,0,0,100,100,0,0,1,3,1,2,64,64,32,1\n"
                                              "Style: Signs,a_FuturaRound,20,&H00FFFFFF,&H000000FF,&H00000000,&H00000000,0,0,0,0,100,100,0,0,1,0,0,5,10,10,10,1\n"
                                              "\n"
                                              "[Graphics]\n"
                                              "SomeFile: /dev/null\n"
                                              "\n"
                                              "[Events]\n"
                                              "Format: Layer, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text\n"
                                              "Dialogue: 0,0:00:00.14,0:00:04.60,Signs,,0000,0000,0000,,This is {\\blur0.3\\fad(500,500)\\fs48\\b1\\c&H6133B5&\\pos(700,700)}Some complex string,{\\pos(1,2)} yo!\n"
                                              "Comment: Some ignored line\n"
                                              "Dialogue: 0,0:00:11.28,0:00:12.50,Default,,0000,0000,0000,,And this is a {\\pos(700,700)}simple {\\otherStyle}one...\n");

static const char* const styles_format_redeclared_file_content = ("[V4+ Styles]\n"
                                                                  "Format: Name, Fontname, Fontsize, Alignment, MarginV\n"
                                                                  "Style: Default,a_FuturaRound,48,2,5\n"
                                                                  "Format: Name, Fontname, Fontsize\n"
                                                                  "Style: Signs,a_FuturaRound,20,2,5\n");

static const char* const styles_invalid_line_file_content = ("[V4+ Styles]\n"
                                                             "Format: Name, Fontname, Fontsize, Alignment, MarginV\n"
                                                             "Valera: O-ho-ho-ho-ho\n"
                                                             "Style: Default,a_FuturaRound,48,2,5\n");

static const char* const styles_invalid_alignment_file_content = ("[V4+ Styles]\n"
                                                                  "Format: Name, Fontname, Fontsize, Alignment, MarginV\n"
                                                                  "Style: Default,a_FuturaRound,48,42,5\n"
                                                                  "Style: Default,a_FuturaRound,48,2,5\n");

static const char* const styles_empty_name_file_content = ("[V4+ Styles]\n"
                                                           "Format: Name, Fontname, Fontsize, Alignment, MarginV\n"
                                                           "Style: ,a_FuturaRound,48,2,5\n"
                                                           "Style: Default,a_FuturaRound,48,2,5\n");

static const char* const events_format_redeclared_file_content = ("[V4+ Styles]\n"
                                                                  "Format: Name, Fontname, Fontsize, Alignment, MarginV\n"
                                                                  "Style: Signs,a_FuturaRound,48,2,5\n"
                                                                  "[Events]\n"
                                                                  "Format: Start, End, Style, MarginV, Text\n"
                                                                  "Dialogue: 0:00:00.14,0:00:04.60,Signs,0,Some text...\n"
                                                                  "Format: Start, End, Style, MarginV, Text\n"
                                                                  "Dialogue: 0:00:00.14,0:00:04.60,Signs,0,Some text #2...\n");

static const char* const events_style_undefined_file_content = ("[V4+ Styles]\n"
                                                                "Format: Name, Fontname, Fontsize, Alignment, MarginV\n"
                                                                "Style: Signs,a_FuturaRound,48,2,5\n"
                                                                "[Events]\n"
                                                                "Format: Start, End, Style, MarginV, Text\n"
                                                                "Dialogue: 0:00:00.14,0:00:04.60,UnknownStyle,0,Some text...\n"
                                                                "Dialogue: 0:00:00.14,0:00:04.60,Signs,0,Some text #2...\n");

static const char* const events_exclude_signs_content = ("[V4+ Styles]\n"
                                                         "Format: Name, Fontname, Fontsize, Alignment, MarginV\n"
                                                         "Style: Signs,a_FuturaRound,48,2,5\n"
                                                         "Style: Default,a_FuturaRound,48,2,0\n"
                                                         "[Events]\n"
                                                         "Format: Start, End, Style, MarginV, Text\n"
                                                         "Dialogue: 0:00:00.00,0:00:01.00,Default,0,Some text...\n"
                                                         "Dialogue: 0:00:00.14,0:00:04.60,Signs,0,Some text #2...\n");

static const char* const events_exclude_styles_content = ("[V4+ Styles]\n"
                                                          "Format: Name, Fontname, Fontsize, Alignment, MarginV\n"
                                                          "Style: StyleA,a_FuturaRound,48,2,5\n"
                                                          "Style: StyleB,a_FuturaRound,48,2,5\n"
                                                          "[Events]\n"
                                                          "Format: Start, End, Style, MarginV, Text\n"
                                                          "Dialogue: 0:00:00.00,0:00:01.00,StyleA,0,Some text...\n"
                                                          "Dialogue: 0:00:00.14,0:00:04.60,StyleB,0,Some text #2...\n");

static const subtitles_t good_file_result {
    {
        140,
        4600,
        {
            {
                0.5,
                0,
                "This is",
            },
            {
                0.027777778,
                1,
                "Some complex string,",
            },
            {
                0.997222245,
                2,
                "yo!",
            },
        },
    },
    {
        11280,
        12500,
        {
            {
                0.0444444455,
                0,
                "And this is a",
            },
            {
                0.027777791,
                1,
                "simple",
            },
            {
                0.027777791,
                2,
                "one...",
            },
        },
    },
};

static const subtitles_t excluded_signs_result {
    {
        0,
        1000,
        {
            {
                0.0,
                0,
                "Some text...",
            },
        },
    }
};

static const subtitles_t leave_only_styles_result {
    {
        140,
        4600,
        {
            {
                0.0,
                0,
                "Some text #2...",
            },
        },
    }
};

static const subtitles_t exclude_styles_result {
    {
        0,
        1000,
        {
            {
                0.0,
                0,
                "Some text...",
            },
        },
    }
};

static std::ispanstream c_str_to_stream(const char* input)
{
    const size_t size = strlen(input);
    const std::span<char> input_span(const_cast<char*>(input), size);

    return std::ispanstream(input_span);
}

TEST(Ass, TestParseFileNormal)
{
    auto test_file = c_str_to_stream(good_file_content);
    auto res = ass::parse_ass_stream(test_file);
    ASSERT_EQ(res, good_file_result) << "Subtitles did not match. Actual result: " << strutils::subtitles_to_string(res);
}

TEST(Ass, TestParseFileWithStylesFormatRedeclared)
{
    auto test_file = c_str_to_stream(styles_format_redeclared_file_content);
    ASSERT_THROW_MSG(ass::parse_ass_stream(test_file), std::runtime_error, "ASS parser error at line 4: Redeclaration of styles section format");
}

TEST(Ass, TestParseFileWithInvalidStylesLine)
{
    auto test_file = c_str_to_stream(styles_invalid_line_file_content);
    ASSERT_THROW_MSG(ass::parse_ass_stream(test_file), std::runtime_error, "ASS parser error at line 3: Invalid styles line type");
}

TEST(Ass, TestParseFileWithInvalidAlignment)
{
    auto test_file = c_str_to_stream(styles_invalid_alignment_file_content);
    ASSERT_THROW_MSG(ass::parse_ass_stream(test_file), std::runtime_error, "ASS parser error at line 3: Invalid alignment 42");
}

TEST(Ass, TestParseFileWithEmptyName)
{
    auto test_file = c_str_to_stream(styles_empty_name_file_content);
    ASSERT_THROW_MSG(ass::parse_ass_stream(test_file), std::runtime_error, "ASS parser error at line 3: Empty style name");
}

TEST(Ass, TestParseFileWithEventsFormatRedeclared)
{
    auto test_file = c_str_to_stream(events_format_redeclared_file_content);
    ASSERT_THROW_MSG(ass::parse_ass_stream(test_file), std::runtime_error, "ASS parser error at line 7: Redeclaration of events section format");
}

TEST(Ass, TestParseFileWithUndefinedStyle)
{
    auto test_file = c_str_to_stream(events_style_undefined_file_content);
    ASSERT_THROW_MSG(ass::parse_ass_stream(test_file), std::runtime_error, "ASS parser error at line 6: Style UnknownStyle is not defined");
}

TEST(Ass, TestParseFileExcludeSigns)
{
    auto test_file = c_str_to_stream(events_exclude_signs_content);
    auto res = ass::parse_ass_stream(test_file, {}, {}, true);
    ASSERT_EQ(res, excluded_signs_result) << "Subtitles did not match. Actual result: " << strutils::subtitles_to_string(res);
}

TEST(Ass, TestParseFileLeaveOnlyStyles)
{
    auto test_file = c_str_to_stream(events_exclude_styles_content);
    auto res = ass::parse_ass_stream(test_file, { "StyleB" }, {}, false);
    ASSERT_EQ(res, leave_only_styles_result) << "Subtitles did not match. Actual result: " << strutils::subtitles_to_string(res);
}

TEST(Ass, TestParseFileExcludeStyles)
{
    auto test_file = c_str_to_stream(events_exclude_styles_content);
    auto res = ass::parse_ass_stream(test_file, {}, { "StyleB" }, false);
    ASSERT_EQ(res, exclude_styles_result) << "Subtitles did not match. Actual result: " << strutils::subtitles_to_string(res);
}

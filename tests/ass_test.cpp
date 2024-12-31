#include <span>
#include <spanstream>
#include <stdexcept>
#include <fstream>
#include <string.h>
#include <gtest/gtest.h>
#include "ass.hpp"

using namespace ass2srt;

#define ASSERT_THROW_MSG(EXPR, EXC_CLS, MSG) \
    do { \
        try { \
            (EXPR); \
            FAIL() << "Exception " << #EXC_CLS << " was not thrown"; \
        } catch (const EXC_CLS &e) { \
            ASSERT_EQ(std::string(e.what()), std::string(MSG)) << "Exception message did not match"; \
        } catch (...) { \
            FAIL() << "Exception other than " << #EXC_CLS << " was thrown"; \
        } \
    } while (0);

static const char *good_file_content = (
    "\xEF\xBB\xBF[Script Info]\n"
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
    "Dialogue: 0,0:00:11.28,0:00:12.50,Default,,0000,0000,0000,,And this is a simple one...\n"
);

static const char *styles_format_redeclared_file_content = (
    "[V4+ Styles]\n"
    "Format: Name, Fontname, Fontsize, Alignment, MarginV\n"
    "Style: Default,a_FuturaRound,48,2,5\n"
    "Format: Name, Fontname, Fontsize\n"
    "Style: Signs,a_FuturaRound,20,2,5\n"
);

static const char *styles_invalid_line_file_content = (
    "[V4+ Styles]\n"
    "Format: Name, Fontname, Fontsize, Alignment, MarginV\n"
    "Valera: O-ho-ho-ho-ho\n"
    "Style: Default,a_FuturaRound,48,2,5\n"
);


static const char *styles_invalid_alignment_file_content = (
    "[V4+ Styles]\n"
    "Format: Name, Fontname, Fontsize, Alignment, MarginV\n"
    "Style: Default,a_FuturaRound,48,42,5\n"
    "Style: Default,a_FuturaRound,48,2,5\n"
);

static const char *styles_empty_name_file_content = (
    "[V4+ Styles]\n"
    "Format: Name, Fontname, Fontsize, Alignment, MarginV\n"
    "Style: ,a_FuturaRound,48,2,5\n"
    "Style: Default,a_FuturaRound,48,2,5\n"
);

static const char *events_format_redeclared_file_content = (
    "[V4+ Styles]\n"
    "Format: Name, Fontname, Fontsize, Alignment, MarginV\n"
    "Style: Signs,a_FuturaRound,48,2,5\n"
    "[Events]\n"
    "Format: Start, End, Style, MarginV, Text\n"
    "Dialogue: 0:00:00.14,0:00:04.60,Signs,0,Some text...\n"
    "Format: Start, End, Style, MarginV, Text\n"
    "Dialogue: 0:00:00.14,0:00:04.60,Signs,0,Some text #2...\n"
);

static const char *events_style_undefined_file_content = (
    "[V4+ Styles]\n"
    "Format: Name, Fontname, Fontsize, Alignment, MarginV\n"
    "Style: Signs,a_FuturaRound,48,2,5\n"
    "[Events]\n"
    "Format: Start, End, Style, MarginV, Text\n"
    "Dialogue: 0:00:00.14,0:00:04.60,UnknownStyle,0,Some text...\n"
    "Format: Start, End, Style, MarginV, Text\n"
    "Dialogue: 0:00:00.14,0:00:04.60,Signs,0,Some text #2...\n"
);

static const subtitles_t good_file_result {
    {
        140,
        4600,
        {
            {
                0.5,
                "This is ",
            },
            {
                0.972222209,
                "Some complex string,",
            },
            {
                0.00277777785,
                " yo!",
            },
        },
    },
    {
        11280,
        12500,
        {
            {
                0.0,
                "And this is a simple one...",
            },
        },
    },
};

static std::ispanstream c_str_to_stream(const char *input)
{
    size_t size = strlen(input);
    std::span<char> input_span(const_cast<char*>(input), size);

    return std::ispanstream(input_span);
}

static std::string subtitles_to_string(const subtitles_t &subtitles)
{
    const std::string indent("    ");
    const std::string indent2 = indent + indent;
    const std::string indent3 = indent2 + indent;
    const std::string indent4 = indent3 + indent;

    std::string output("{\n");
    for (auto subline : subtitles) {
        output += indent + "{\n";
        output += indent2 + std::to_string(subline.start_milis) + ",\n";
        output += indent2 + std::to_string(subline.end_milis) + ",\n";
        output += indent2 + "{\n";
        for (auto part : subline.parts) {
            output += indent3 + "{\n";
            output += indent4 + std::to_string(part.v_pos) + ",\n";
            output += indent4 + "\"" + part.text + "\",\n";
            output += indent3 + "},\n";
        }
        output += indent2 + "},\n";
        output += indent + "},\n";
    }
    output += "}";
    return output;
}

TEST(ass, test_parse_file_normal) {
    auto test_file = c_str_to_stream(good_file_content);
    auto res = ass::parse_ass_stream(test_file);
    ASSERT_EQ(res, good_file_result) << "Subtitles did not match. Actual result: " << subtitles_to_string(res);
}

TEST(ass, test_parse_file_with_styles_format_redeclared) {
    auto test_file = c_str_to_stream(styles_format_redeclared_file_content);
    ASSERT_THROW_MSG(ass::parse_ass_stream(test_file), std::runtime_error, "ASS parser error at line 4: Redeclaration of styles section format");
}

TEST(ass, test_parse_file_with_invalid_styles_line) {
    auto test_file = c_str_to_stream(styles_invalid_line_file_content);
    ASSERT_THROW_MSG(ass::parse_ass_stream(test_file), std::runtime_error, "ASS parser error at line 3: Invalid styles line type");
}

TEST(ass, test_parse_file_with_invalid_alignment) {
    auto test_file = c_str_to_stream(styles_invalid_alignment_file_content);
    ASSERT_THROW_MSG(ass::parse_ass_stream(test_file), std::runtime_error, "ASS parser error at line 3: Invalid alignment 42");
}

TEST(ass, test_parse_file_with_empty_name) {
    auto test_file = c_str_to_stream(styles_empty_name_file_content);
    ASSERT_THROW_MSG(ass::parse_ass_stream(test_file), std::runtime_error, "ASS parser error at line 3: Empty style name");
}

TEST(ass, test_parse_file_with_events_format_redeclared) {
    auto test_file = c_str_to_stream(events_format_redeclared_file_content);
    ASSERT_THROW_MSG(ass::parse_ass_stream(test_file), std::runtime_error, "ASS parser error at line 7: Redeclaration of events section format");
}

TEST(ass, test_parse_file_with_undefined_style) {
    auto test_file = c_str_to_stream(events_style_undefined_file_content);
    ASSERT_THROW_MSG(ass::parse_ass_stream(test_file), std::runtime_error, "ASS parser error at line 6: Style UnknownStyle is not defined");
}

#include <span>
#include <spanstream>
#include <fstream>
#include <string.h>
#include <gtest/gtest.h>
#include "ass.hpp"

using namespace ass2srt;

static const char *good_file_content = (
    "[Script Info]\n"
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
    "[Events]\n"
    "Format: Layer, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text\n"
    "Dialogue: 0,0:00:00.14,0:00:04.60,Signs,,0000,0000,0000,,This is {\\blur0.3\\fad(500,500)\\fs48\\b1\\c&H6133B5&\\pos(700,700)}Some complex string,{\\pos(1,2)} yo!\n"
    "Dialogue: 0,0:00:11.28,0:00:12.50,Default,,0000,0000,0000,,And this is a simple one...\n"
);

static std::ispanstream c_str_to_stream(const char *input)
{
    size_t size = strlen(input);
    std::span<char> input_span(const_cast<char*>(input), size);

    return std::ispanstream(input_span);
}

TEST(ass, parse_file_normal) {
    auto test_file = c_str_to_stream(good_file_content);

    auto res = ass::parse_ass_stream(test_file);
    ASSERT_GT(res.size(), 0);
}

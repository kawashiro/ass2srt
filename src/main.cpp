#include <fstream>
#include <stdio.h>
#include "argparser.hpp"
#include "ass.hpp"
#include "config.h"

using namespace ass2srt;

int main(int argc, char **argv)
{
    const argparser::args params = argparser::parse_args(argc, argv);
    if (!params.is_valid()) {
        printf("ass2srt converter v.%d.%d.%d\n", ASS2SRT_VERSION_MAJOR, ASS2SRT_VERSION_MINOR, ASS2SRT_VERSION_PATCH);
        printf("Usage: ass2srt -i input_file -o output_file [-s styles_to_display] [-e excluded_styles] [ -x ]\n");
        return 2;
    }

    // Just a simple test ...
    std::ifstream input_file(params.input_file, std::ios_base::in);
    auto lines = ass::parse_ass_stream(input_file);
    for (auto line : lines) {
        for (auto part : line.parts) {
            printf("==>\n  [%f] %s\n", part.v_pos, part.text.c_str());
        }
    }

    return 0;
}

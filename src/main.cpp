#include "argparser.hpp"
#include "ass.hpp"
#include "config.h"
#include "merge.hpp"
#include "srt.hpp"
#include "strutils.hpp"
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace ass2srt;

auto main(int argc, char** argv) -> int
{
    const argparser::args params = argparser::parse_args(argc, argv);
    if (!params.is_valid()) {
        printf("ass2srt converter v%d.%d.%d\n", ASS2SRT_VERSION_MAJOR, ASS2SRT_VERSION_MINOR, ASS2SRT_VERSION_PATCH);
        printf("Usage: ass2srt -i input_file -o output_file [-s styles_to_display] [-e excluded_styles] [ -x ]\n");
        return 2;
    }

    try {
        std::ifstream input_file(params.input_file, std::ios_base::in);
        if (input_file.fail()) {
            throw std::runtime_error(strutils::format("Failed to open input file %s: %s", params.input_file.c_str(), std::strerror(errno)));
        }

        std::ofstream output_file(params.output_file, std::ios_base::out | std::ios_base::binary);
        if (output_file.fail()) {
            throw std::runtime_error(strutils::format("Failed to open output file %s: %s", params.output_file.c_str(), std::strerror(errno)));
        }

        auto raw_lines = ass::parse_ass_stream(input_file, params.styles_scope, params.excluded_styles, params.exclude_signs);
        auto merged_lines = merge::merge_subtitles_parts(raw_lines);
        srt::write_srt_stream(merged_lines, output_file);

    } catch (const std::runtime_error& e) {
        printf("FATAL: %s\n", e.what());
        return 1;
    }

    return 0;
}

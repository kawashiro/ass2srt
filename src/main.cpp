#include "argparser.hpp"
#include "ass.hpp"
#include "config.h"
#include "io.hpp"
#include "merge.hpp"
#include "srt.hpp"
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <stdexcept>

using namespace ass2srt;

auto main(int argc, char** argv) -> int
{
    auto params = argparser::parse_args(argc, argv);
    if (params.show_help || !params.valid()) {
        fprintf(stderr, "ass2srt converter v%d.%d.%d\n", ASS2SRT_VERSION_MAJOR, ASS2SRT_VERSION_MINOR, ASS2SRT_VERSION_PATCH);
        fprintf(stderr, "Usage: %s [ -i input_file ] [ -o output_file ] [-s styles_to_display] [-e excluded_styles] [ -x ] [ -h ]\n", argv[0]);
        return params.show_help ? 0 : 2;
    }

    try {
        auto input_file = io::open_istream(params.input_file);

        auto raw_lines = ass::parse_ass_stream(*input_file, params.styles_scope, params.excluded_styles, params.exclude_signs);
        auto merged_lines = merge::merge_subtitles_parts(raw_lines);

        auto output_file = io::open_ostream(params.output_file);
        srt::write_srt_stream(merged_lines, *output_file);

    } catch (const std::runtime_error& e) {
        fprintf(stderr, "FATAL: %s\n", e.what());
        return 1;
    }

    return 0;
}

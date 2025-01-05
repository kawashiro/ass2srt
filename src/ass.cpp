#include <istream>
#include <stdexcept>
#include <string>
#include "ass.hpp"
#include "ass/parser.hpp"
#include "strutils.hpp"
#include "subline.hpp"

using namespace ass2srt;

subtitles_t ass::parse_ass_stream(std::istream &input)
{
    subtitles_t subtitles;

    ass::parser::ass_res_t value(input, subtitles);
    auto initial_state = new ass::parser::InitialState();

    try {
        statemachine::run<ass::parser::ass_res_t>(initial_state, value);
    } catch (const std::invalid_argument &e) {
        throw std::runtime_error(strutils::format("ASS parser error at line %d: %s", value.line_no, e.what()));
    }

    return value.result;
}

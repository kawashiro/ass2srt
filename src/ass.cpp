#include <istream>
#include <stdexcept>
#include <string>
#include "ass.hpp"
#include "ass/parserstate.hpp"
#include "strutils.hpp"
#include "subline.hpp"

using namespace ass2srt;

subtitles_t ass::parse_ass_stream(std::istream &input)
{
    subtitles_t subtitles;

    ass::parserstate::ass_res_t value(input, subtitles);
    auto initial_state = new ass::parserstate::InitialState();

    try {
        statemachine::run<ass::parserstate::ass_res_t>(initial_state, value);
    } catch (const std::invalid_argument &e) {
        throw std::runtime_error(strutils::format("ASS parser error at line %d: %s", value.line_no, e.what()));
    }

    return value.result;
}

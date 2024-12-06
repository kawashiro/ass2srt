#include <istream>
#include <string>
#include "ass.hpp"
#include "subline.hpp"
#include "ass/parserstate.hpp"

using namespace ass2srt;

subtitles_t&& ass::parse_ass_stream(std::istream &input)
{
    subtitles_t subtitles;

    ass::parserstate::ass_res_t value(input, subtitles);
    auto initial_state = new ass::parserstate::InitialState();

    statemachine::run<ass::parserstate::ass_res_t>(initial_state, value);
    return std::move(value.result);
}

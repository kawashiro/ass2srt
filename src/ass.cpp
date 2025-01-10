#include "ass.hpp"
#include "ass/parser.hpp"
#include "statemachine.hpp"
#include "strutils.hpp"
#include "subline.hpp"
#include <istream>
#include <set>
#include <stdexcept>
#include <string>

using namespace ass2srt;

auto ass::parse_ass_stream(std::istream& input, const std::set<std::string>& styles_scope,
    const std::set<std::string>& exclued_style, const bool exclude_signs) -> subtitles_t
{
    subtitles_t subtitles;

    ass::parser::ass_res_t value(input, subtitles, styles_scope, exclued_style, exclude_signs);
    auto* initial_state = new ass::parser::InitialState();

    try {
        statemachine::run<ass::parser::ass_res_t>(initial_state, value);
    } catch (const std::invalid_argument& e) {
        throw std::runtime_error(strutils::format("ASS parser error at line %d: %s", value.line_no, e.what()));
    }

    return value.result;
}

auto ass::parse_ass_stream(std::istream& input) -> subtitles_t
{
    return ass::parse_ass_stream(input, {}, {}, false);
}

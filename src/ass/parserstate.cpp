#include <istream>
#include <memory>
#include <string>
#include "parserstate.hpp"
#include "section.hpp"
#include "../strutils.hpp"

using namespace ass2srt::ass::parserstate;

typedef ass2srt::ass::section::Section Section;

#define PARSERSTATE_HANDLE_DUMMY_LINE(C, V)                    \
    do {                                                       \
        if ((V).eof) {                                         \
            return std::make_unique<FinalState>();             \
        }                                                      \
        if ((V).token.empty() || (V).token.starts_with(";")) { \
            return std::make_unique<C>();                      \
        }                                                      \
    } while (0)

static void get_whole_line_token(ass_res_t &value)
{
    std::string line;
    if (std::getline(value.istream, line)) {
        ass2srt::strutils::trim(line);
        value.token = line;
    } else {
        value.token = "";
        value.eof = true;
    }
}

static inline Section parse_section(std::string &value)
{
    return ass2srt::ass::section::parse(value);
}

ass_res_t::ass_res_t(std::istream &istream_, subtitles_t &result_):
    istream(istream_),
    result(result_),
    token(""),
    eof(false)
{
}

ass_res_t& ass_res_t::operator=(ass_res_t& value)
{
    return value;
}

// InitialState
std::unique_ptr<StateType> InitialState::transition(ass_res_t &value)
{
    PARSERSTATE_HANDLE_DUMMY_LINE(InitialState, value);

    switch (parse_section(value.token)) {
    case Section::SCRIPT_INFO:
        return std::make_unique<ScriptInfoSectionState>();

    default:
        // TODO: Raise an error
        return std::make_unique<InitialState>();
    }
}

void InitialState::output(ass_res_t &value)
{
    get_whole_line_token(value);
}

// ScriptInfoSectionState
std::unique_ptr<StateType> ScriptInfoSectionState::transition(ass_res_t &value)
{
    // TODO: Implement
    return std::make_unique<FinalState>();
}

void ScriptInfoSectionState::output(ass_res_t &value)
{
    // TODO: Implement
}

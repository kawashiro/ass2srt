#include <istream>
#include <memory>
#include <stdexcept>
#include <stdio.h>
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

#define PARSERSTATE_HANDLE_SECTION_BREAK(V)          \
    do {                                             \
        try {                                        \
            auto section = parse_section((V).token); \
            return get_state_for_section(section);   \
        } catch (const std::invalid_argument &e) {   \
        }                                            \
    } while (0)

#define STATE_PTR(CLS) std::make_unique<CLS>()

static void get_whole_line_token(ass_res_t &value)
{
    std::string line;
    if (std::getline(value.istream, line)) {
        ass2srt::strutils::trim(line);
        value.token = line;
        value.line_no++;
    } else {
        value.token = "";
        value.eof = true;
    }
}

static inline Section parse_section(std::string &value)
{
    return ass2srt::ass::section::parse(value);
}

std::unique_ptr<StateType> get_state_for_section(Section section)
{
    switch (section) {
    case Section::SCRIPT_INFO:
        return STATE_PTR(ScriptInfoSectionState);

    case Section::STYLES:
    case Section::EVENTS:
    case Section::UNKNOWN:
        return STATE_PTR(UnsupportedSectionState);
    }

    // Actually unreachable, but makes GCC happy
    return STATE_PTR(UnsupportedSectionState);
}

ass_res_t::ass_res_t(std::istream &istream_, subtitles_t &result_):
    istream(istream_),
    result(result_),
    token(""),
    v_size(-1),
    line_no(0),
    eof(false)
{
}

// InitialState
std::unique_ptr<StateType> InitialState::transition(ass_res_t &value)
{
    PARSERSTATE_HANDLE_DUMMY_LINE(InitialState, value);

    try {
        auto section = parse_section(value.token);
        return get_state_for_section(section);
    } catch (const std::invalid_argument &e) {
        throw std::runtime_error(ass2srt::strutils::format("%s at line %d (%s)", e.what(), value.line_no, value.token));
    }
}

void InitialState::output(ass_res_t &value)
{
    get_whole_line_token(value);
}

// ScriptInfoSectionState
std::unique_ptr<StateType> ScriptInfoSectionState::transition(ass_res_t &value)
{
    PARSERSTATE_HANDLE_DUMMY_LINE(ScriptInfoSectionState, value);
    PARSERSTATE_HANDLE_SECTION_BREAK(value);

    size_t separator_pos = value.token.find(":", 0);
    if (separator_pos == std::string::npos) {
        throw std::runtime_error(ass2srt::strutils::format("Invalid script info record at line %d (%s)", value.line_no, value.token));
    }

    auto key = value.token.substr(0, separator_pos);
    if (key == "PlayResY") {
        // Only this is needed for us
        auto res_y = value.token.substr(separator_pos + 1, value.token.length() - separator_pos - 1);
        int v_size = std::stoi(res_y);
        value.v_size = v_size;
    }

    return STATE_PTR(ScriptInfoSectionState);
}

void ScriptInfoSectionState::output(ass_res_t &value)
{
    get_whole_line_token(value);
}

// UnsupportedSectionState
std::unique_ptr<StateType> UnsupportedSectionState::transition(ass_res_t &value)
{
    PARSERSTATE_HANDLE_DUMMY_LINE(ScriptInfoSectionState, value);
    PARSERSTATE_HANDLE_SECTION_BREAK(value);

    return STATE_PTR(UnsupportedSectionState);
}

void UnsupportedSectionState::output(ass_res_t &value)
{
    get_whole_line_token(value);
}

#include <istream>
#include <list>
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

static std::string get_line_value(std::string &input)
{
    size_t separator_pos = input.find(":", 0);
    auto line = input.substr(separator_pos + 1, input.length() - separator_pos - 1);
    ass2srt::strutils::trim(line);

    return line;
}

static inline Section parse_section(std::string &value)
{
    return ass2srt::ass::section::parse(value);
}

std::unique_ptr<StateType> get_state_for_section(Section section)
{
    std::unique_ptr<StateType> res;

    switch (section) {
    case Section::SCRIPT_INFO:
        res = STATE_PTR(ScriptInfoSectionState);
        break;

    case Section::STYLES:
        res = STATE_PTR(StylesSectionState);
        break;

    case Section::EVENTS:
    case Section::UNKNOWN:
        res = STATE_PTR(UnsupportedSectionState);
    }

    return res;
}

ass_res_t::ass_res_t(std::istream &istream_, subtitles_t &result_):
    istream(istream_),
    result(result_),
    token(""),
    styles_format({}),
    styles_spec({}),
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

// StylesSectionState
std::unique_ptr<StateType> StylesSectionState::transition(ass_res_t &value)
{
    PARSERSTATE_HANDLE_DUMMY_LINE(ScriptInfoSectionState, value);
    PARSERSTATE_HANDLE_SECTION_BREAK(value);

    size_t separator_pos = value.token.find(":", 0);
    if (separator_pos == std::string::npos) {
        throw std::runtime_error(ass2srt::strutils::format("Invalid styles line at line %d (%s)", value.line_no, value.token));
    }

    auto line_type = value.token.substr(0, separator_pos);
    if (line_type == "Format") {
        if (!value.styles_format.empty()) {
            throw std::runtime_error(ass2srt::strutils::format("Redeclaration of styles section format at line %d (%s)", value.line_no, value.token));
        }
        return STATE_PTR(StylesFormatState);
    } else if (line_type == "Style") {
        return STATE_PTR(StyleSpecState);
    }

    throw std::runtime_error(ass2srt::strutils::format("Invalid styles line type at line %d (%s)", value.line_no, value.token));
}

void StylesSectionState::output(ass_res_t &value)
{
    get_whole_line_token(value);
}

// StylesFormatState
std::unique_ptr<StateType> StylesFormatState::transition(ass_res_t &value)
{
    std::list<std::string> fields;
    std::string input = value.token;
    size_t pos = 0;
    std::string token;
    while ((pos = input.find(',')) != std::string::npos) {
        token = input.substr(0, pos);
        ass2srt::strutils::trim(token);
        fields.push_back(token);
        input.erase(0, pos + 1);
    }
    ass2srt::strutils::trim(input);
    fields.push_back(input);

    value.styles_format = fields;

    return STATE_PTR(StylesSectionState);
}

void StylesFormatState::output(ass_res_t &value)
{
    value.token = get_line_value(value.token);
}

// StyleSpecState
std::unique_ptr<StateType> StyleSpecState::transition(ass_res_t &value)
{
    size_t str_pos = 0;
    ass_res_t::styles_spec_t style { 2, 0 };
    std::string style_name = "";
    for (auto field : value.styles_format) {
        if (field == "Name") {
            style_name = value.token.substr(str_pos, value.token.find(',', str_pos) - str_pos);

        } else if (field == "Alignment") {
            auto alignment = value.token.substr(str_pos, value.token.find(',', str_pos) - str_pos);
            int alignment_i = std::stoi(alignment);
            if (alignment_i < 1 || alignment_i > 11) {
                throw std::runtime_error(ass2srt::strutils::format("Invalid alignment at line %d (%s)", value.line_no, alignment));
            }
            style.alignment = alignment_i;
            
        } else if (field == "MarginV") {
            auto margin_v = value.token.substr(str_pos, value.token.find(',', str_pos) - str_pos);
            int margin_v_i = std::stoi(margin_v);
            style.margin_v = margin_v_i;
        }
        str_pos = value.token.find(',', str_pos) + 1;
    }
    
    if (style_name.empty()) {
        throw std::runtime_error(ass2srt::strutils::format("Empty style name at line %d (%s)", value.line_no, value.token));
    }
    value.styles_spec[style_name] = style;

    return STATE_PTR(StylesSectionState);
}

void StyleSpecState::output(ass_res_t &value)
{
    value.token = get_line_value(value.token);
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

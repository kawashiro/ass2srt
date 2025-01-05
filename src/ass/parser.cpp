#include <cstring>
#include <istream>
#include <list>
#include <memory>
#include <set>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include "parser.hpp"
#include "field.hpp"
#include "line.hpp"
#include "section.hpp"
#include "vpos.hpp"
#include "../strutils.hpp"

using namespace ass2srt::ass::parser;

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

#define PARSERSTATE_HANDLE_SECTION_BREAK(V)                \
    do {                                                   \
        try {                                              \
            auto section = ass::section::parse((V).token); \
            return get_state_for_section(section);         \
        } catch (const std::invalid_argument &e) {         \
        }                                                  \
    } while (0)

#define STATE_PTR(CLS) std::make_unique<CLS>()

const unsigned char bom[3] = {0xEF, 0xBB, 0xBF};

static void get_whole_line_token(ass_res_t &value)
{
    std::string line;
    if (std::getline(value.istream, line)) {
        if (value.line_no == 0 && std::memcmp(line.c_str(), bom, 3) == 0) {
            line.erase(0, 3);
        }
        ass2srt::strutils::trim(line);
        value.token = line;
        value.line_no++;
    } else {
        value.token = "";
        value.eof = true;
    }
}

static std::unique_ptr<StateType> get_state_for_section(const Section section)
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
        res = STATE_PTR(EventsSectionState);
        break;

    case Section::UNKNOWN:
        res = STATE_PTR(UnsupportedSectionState);
    }

    return res;
}

ass_res_t::ass_res_t(std::istream &istream,subtitles_t &result, const std::set<std::string> &styles_scope,
    const std::set<std::string> &excluded_styles, const bool exclude_signs):
    istream(istream),
    result(result),
    token(""),
    styles_format({}),
    events_format({}),
    styles_spec({}),
    styles_scope(styles_scope),
    excluded_styles(excluded_styles),
    exclude_signs(exclude_signs),
    v_size(-1),
    line_no(0),
    eof(false)
{
}

// InitialState
std::unique_ptr<StateType> InitialState::transition(ass_res_t &value)
{
    PARSERSTATE_HANDLE_DUMMY_LINE(InitialState, value);

    auto section = ass::section::parse(value.token);
    return get_state_for_section(section);
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

    std::string res_y;
    auto line_type = ass::line::parse_type(value.token);

    // Only this is needed for us
    if (line_type == line::PLAY_RES_Y) {
        auto res_y = ass::line::get_line_value(value.token);
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
    PARSERSTATE_HANDLE_DUMMY_LINE(StylesSectionState, value);
    PARSERSTATE_HANDLE_SECTION_BREAK(value);

    auto line_type = ass::line::parse_type(value.token);
    switch (line_type) {
    case line::FORMAT:
        if (!value.styles_format.empty()) {
            throw std::invalid_argument("Redeclaration of styles section format");
        }
        return STATE_PTR(StylesFormatState);

    case line::STYLE:
        return STATE_PTR(StyleSpecState);
    
    default:
        throw std::invalid_argument("Invalid styles line type");
    }
}

void StylesSectionState::output(ass_res_t &value)
{
    get_whole_line_token(value);
}

// StylesFormatState
std::unique_ptr<StateType> StylesFormatState::transition(ass_res_t &value)
{
    value.styles_format = ass::line::parse_format_declaration(value.token, {});
    return STATE_PTR(StylesSectionState);
}

void StylesFormatState::output(ass_res_t &value)
{
    value.token = ass::line::get_line_value(value.token);
}

// StyleSpecState
std::unique_ptr<StateType> StyleSpecState::transition(ass_res_t &value)
{
    field::LineValuesParser parser;
    
    parser.on<std::string>(field::NAME, [](std::string &value) {
        return new std::string(value);
    });

    parser.on<int>(field::ALIGNMENT, [](std::string &value) {
        auto alignment_i = std::stoi(value);
        if (!ALIGN_VALID(alignment_i)) {
            throw std::invalid_argument(strutils::format("Invalid alignment %s", value.c_str()));
        }
        return new int(alignment_i);
    });

    parser.on<int>(field::MARGIN_V, [](std::string &value) {
        auto margin_v_i = std::stoi(value);
        return new int(margin_v_i);
    });

    parser.parse(value.styles_format, value.token);

    auto style_name = parser.get<std::string>(field::NAME);
    if (style_name.empty()) {
        throw std::invalid_argument("Empty style name");
    }
    auto alignment = parser.get<int>(field::ALIGNMENT);
    auto margin_v = parser.get<int>(field::MARGIN_V);

    value.styles_spec[style_name] = {static_cast<uint8_t>(alignment), margin_v, -1};

    return STATE_PTR(StylesSectionState);
}

void StyleSpecState::output(ass_res_t &value)
{
    value.token = ass::line::get_line_value(value.token);
}

// EventsSectionState
std::unique_ptr<StateType> EventsSectionState::transition(ass_res_t &value)
{
    PARSERSTATE_HANDLE_DUMMY_LINE(EventsSectionState, value);
    PARSERSTATE_HANDLE_SECTION_BREAK(value);

    auto line_type = ass::line::parse_type(value.token);
    switch (line_type) {
    case line::FORMAT:
        if (!value.events_format.empty()) {
            throw std::invalid_argument("Redeclaration of events section format");
        }
        return STATE_PTR(EventsFormatState);

    case line::DIALOGUE:
        return STATE_PTR(EventDialogueLineState);
    
    default:
        return STATE_PTR(EventIgnoredLineState);
    }
}

void EventsSectionState::output(ass_res_t &value)
{
    get_whole_line_token(value);
}

// EventsFormatState
std::unique_ptr<StateType> EventsFormatState::transition(ass_res_t &value)
{
    value.events_format = ass::line::parse_format_declaration(value.token, {"Start", "End", "Text"});
    return STATE_PTR(EventsSectionState);
}

void EventsFormatState::output(ass_res_t &value)
{
    value.token = ass::line::get_line_value(value.token);
}

// EventDialogueLineState
std::unique_ptr<StateType> EventDialogueLineState::transition(ass_res_t &value)
{
    field::LineValuesParser parser;
    
    parser.on<long>(field::START, [](std::string &value) {
        auto milis = ass::field::parse_time_millis(value);
        return new long(milis);
    });

    parser.on<long>(field::END, [](std::string &value) {
        auto milis = ass::field::parse_time_millis(value);
        return new long(milis);
    });

    parser.on<std::string>(field::STYLE, [](std::string &value) {
        return new std::string(value);
    });

    parser.on<int>(field::MARGIN_V, [](std::string &value) {
        auto margin_v_i = std::stoi(value);
        return new int(margin_v_i);
    });

    parser.on<std::list<ass_res_t::subline_part_t>>(field::TEXT, [](std::string &value) {
        auto parts = new std::list<ass_res_t::subline_part_t>();
        for (size_t part_begin = 0, part_end = 0; part_end + 1 < value.length();) {
            part_end = value.find('{', part_begin + 1);
            if (part_end == std::string::npos) {
                part_end = value.length();
            }
            auto current_part = value.substr(part_begin, part_end - part_begin); // {\style}Text

            ass_res_t::subline_part_t parsed_part;
            if (current_part.at(0) == '{') {
                auto style_end = current_part.find('}') + 1;
                auto style_spec = current_part.substr(0, style_end); // {\style}
                parsed_part.inline_style = ass::field::parse_inline_style(style_spec);
                parsed_part.text = field::parse_plain_text(current_part.substr(style_end, current_part.length() - style_end)); // Text
            } else {
                parsed_part.text = field::parse_plain_text(current_part); // Text
            }

            parts->push_back(parsed_part);
            part_begin = part_end;
        }

        return parts;
    });

    parser.parse(value.events_format, value.token);

    auto start_millis = parser.get<long>(field::START);
    auto end_millis = parser.get<long>(field::END);
    auto margin_v = parser.get<int>(field::MARGIN_V);
    auto text_parts = parser.get<std::list<ass_res_t::subline_part_t>>(field::TEXT);

    auto style_name = parser.get<std::string>(field::STYLE);
    auto style_spec_it = value.styles_spec.find(style_name);
    if (style_spec_it == value.styles_spec.end()) {
        throw std::invalid_argument(strutils::format("Style %s is not defined", style_name.c_str()));
    }
    auto style = style_spec_it->second;

    subline result {start_millis, end_millis, {}};
    for (auto ass_part : text_parts) {
        float v_pos = ass::vpos::calculate_vpos(value.v_size, margin_v, style, ass_part.inline_style);
        result.parts.push_back({v_pos, ass_part.text});
    }

    // Styles-based filtering
    if (!value.styles_scope.empty() && !value.styles_scope.contains(style_name)) {
        return STATE_PTR(EventsSectionState);
    }
    if (!value.excluded_styles.empty() && value.excluded_styles.contains(style_name)) {
        return STATE_PTR(EventsSectionState);
    }
    if (value.exclude_signs && (style_name.contains("sign") || style_name.contains("Sign"))) {
        return STATE_PTR(EventsSectionState);
    }

    value.result.push_back(result);

    return STATE_PTR(EventsSectionState);
}

void EventDialogueLineState::output(ass_res_t &value)
{
    value.token = ass::line::get_line_value(value.token);
}

// EventIgnoredLineState
std::unique_ptr<StateType> EventIgnoredLineState::transition(ass_res_t &)
{
    return STATE_PTR(EventsSectionState);
}

void EventIgnoredLineState::output(ass_res_t &)
{
    // We don't parse here anything so there is nothing to be done
}

// UnsupportedSectionState
std::unique_ptr<StateType> UnsupportedSectionState::transition(ass_res_t &value)
{
    PARSERSTATE_HANDLE_DUMMY_LINE(UnsupportedSectionState, value);
    PARSERSTATE_HANDLE_SECTION_BREAK(value);

    return STATE_PTR(UnsupportedSectionState);
}

void UnsupportedSectionState::output(ass_res_t &value)
{
    get_whole_line_token(value);
}

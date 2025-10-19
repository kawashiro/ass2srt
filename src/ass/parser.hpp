#ifndef ASS2SRT_ASS_PARSERSTATE_H
#define ASS2SRT_ASS_PARSERSTATE_H

#include "../statemachine.hpp"
#include "../subline.hpp"
#include "field.hpp"
#include <istream>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

// NOLINTBEGIN
#define PARSER_STATE_CLASS_DECL(NAME)                                             \
    class NAME : public StateType {                                               \
    public:                                                                       \
        auto transition(ass_res_t& value) -> std::unique_ptr<StateType> override; \
        void output(ass_res_t& value) override;                                   \
    }
// NOLINTEND

namespace ass2srt::ass::parser {
/**
 * Intermediate parsing result
 */
struct ass_res_t {
    struct subline_part_t {
        field::styles_spec_t inline_style;
        std::string text;
    };
    const field::styles_spec_t default_style = { ALIGN_DEFAULT, -1, -1.0F, false };
    std::istream& istream;
    subtitles_t& result;
    std::string token;
    std::vector<field::FieldType> styles_format;
    std::vector<field::FieldType> events_format;
    std::unordered_map<std::string, field::styles_spec_t> styles_spec;
    std::set<std::string> styles_scope;
    std::set<std::string> excluded_styles;
    bool exclude_signs;
    int v_size;
    int line_no;
    bool eof;

    ass_res_t(std::istream&, subtitles_t&, const std::set<std::string>&, const std::set<std::string>&, bool);

    auto get_style(const std::string&) -> field::styles_spec_t;
};

/**
 * Base state class
 */
using StateType = statemachine::State<ass_res_t>;

/**
 * Top level of the file
 */
PARSER_STATE_CLASS_DECL(InitialState);

/**
 * Global script metadata section
 */
PARSER_STATE_CLASS_DECL(ScriptInfoSectionState);

/**
 * Styles section
 */
PARSER_STATE_CLASS_DECL(StylesSectionState);

/**
 * Styles format desc
 */
PARSER_STATE_CLASS_DECL(StylesFormatState);

/**
 * Style line
 */
PARSER_STATE_CLASS_DECL(StyleSpecState);

/**
 * Events section
 */
PARSER_STATE_CLASS_DECL(EventsSectionState);

/**
 * Events format desc
 */
PARSER_STATE_CLASS_DECL(EventsFormatState);

/**
 * Event dialogue line
 */
PARSER_STATE_CLASS_DECL(EventDialogueLineState);

/**
 * Event ignored line (comment or unsupported)
 */
PARSER_STATE_CLASS_DECL(EventIgnoredLineState);

/**
 * It is a valid section declaration but we have no need to support it yet
 */
PARSER_STATE_CLASS_DECL(UnsupportedSectionState);

/**
 * Final state
 */
using FinalState = statemachine::FinalState<ass_res_t>;
}

#endif

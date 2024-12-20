#ifndef _ASS2SRT_ASS_PARSERSTATE_H
#define _ASS2SRT_ASS_PARSERSTATE_H

#include <istream>
#include <list>
#include <memory>
#include <stdint.h>
#include <string>
#include <unordered_map>
#include "field.hpp"
#include "../statemachine.hpp"
#include "../subline.hpp"

#define PARSER_STATE_CLASS_DECL(NAME)                                    \
    class NAME : public StateType                                        \
    {                                                                    \
        public:                                                          \
        virtual std::unique_ptr<StateType> transition(ass_res_t &value); \
        void output(ass_res_t &value);                                   \
    }

// Text align calculation
#define ALIGN_LEFT 1
#define ALIGN_CENTER 2
#define ALIGN_RIGHT 3

#define ALIGN_BOTTOM 0
#define ALIGN_MIDDLE 8
#define ALIGN_TOP 4

#define ALIGN(Y, X) ((Y) + (X))
#define ALIGN_VALID(VAL) ((VAL) >= ALIGN(ALIGN_BOTTOM, ALIGN_LEFT) && (VAL) <= ALIGN(ALIGN_MIDDLE, ALIGN_RIGHT))

#define ALIGN_DEFAULT ALIGN(ALIGN_BOTTOM, ALIGN_CENTER)

namespace ass2srt::ass::parserstate {
    /**
     * Intermediate parsing result
     */
    struct ass_res_t
    {
        struct subline_part_t {
            field::styles_spec_t inline_style;
            std::string text;
        };
        std::istream &istream;
        subtitles_t &result;
        std::string token;
        std::list<field::FieldType> styles_format;
        std::list<field::FieldType> events_format;
        std::unordered_map<std::string, field::styles_spec_t> styles_spec;
        int v_size;
        int line_no;
        bool eof;

        ass_res_t(std::istream &, subtitles_t &);
    };

    /**
     * Base state class
     */
    typedef statemachine::State<ass_res_t> StateType;

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
    typedef statemachine::FinalState<ass_res_t> FinalState;
}

#endif

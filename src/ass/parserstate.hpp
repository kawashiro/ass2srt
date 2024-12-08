#ifndef _ASS2SRT_ASS_PARSERSTATE_H
#define _ASS2SRT_ASS_PARSERSTATE_H

#include <istream>
#include <list>
#include <memory>
#include <stdint.h>
#include <string>
#include <unordered_map>
#include "../statemachine.hpp"
#include "../subline.hpp"

#define PARSER_STATE_CLASS_DECL(NAME, ID)                                \
    class NAME : public StateType                                        \
    {                                                                    \
        public:                                                          \
        static const uint8_t id = (ID);                                  \
        virtual std::unique_ptr<StateType> transition(ass_res_t &value); \
        void output(ass_res_t &value);                                   \
    }

namespace ass2srt::ass::parserstate {
    /**
     * Intermediate parsing result
     */
    struct ass_res_t
    {
        struct styles_spec_t
        {
            uint8_t alignment;
            int margin_v;
        };
        std::istream &istream;
        subtitles_t &result;
        std::string token;
        std::list<std::string> styles_format;
        std::unordered_map<std::string, styles_spec_t> styles_spec;
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
     * Possible states enumeration
     */
    enum State
    {
        INITIAL = 0,

        SCRIPT_INFO,
        STYLES,
        STYLES_FORMAT,
        STYLE_SPEC,
        // ...
        UNSUPPORTED,
        
        FINAL = UINT8_MAX
    };

    /**
     * Top level of the file
     */
    PARSER_STATE_CLASS_DECL(InitialState, INITIAL);

    /**
     * Global script metadata section
     */
    PARSER_STATE_CLASS_DECL(ScriptInfoSectionState, SCRIPT_INFO);

    /**
     * Styles section
     */
    PARSER_STATE_CLASS_DECL(StylesSectionState, STYLES);

    /**
     * Styles format desc
     */
    PARSER_STATE_CLASS_DECL(StylesFormatState, STYLES_FORMAT);

    /**
     * Style line
     */
    PARSER_STATE_CLASS_DECL(StyleSpecState, STYLE_SPEC);

    /**
     * It is a valid section declaration but we have no need to support it yet
     */
    PARSER_STATE_CLASS_DECL(UnsupportedSectionState, UNSUPPORTED);

    /**
     * Final state
     */
    typedef statemachine::FinalState<ass_res_t> FinalState;
}

#endif

#ifndef _ASS2SRT_ASS_PARSERSTATE_H
#define _ASS2SRT_ASS_PARSERSTATE_H

#include <istream>
#include <memory>
#include <stdint.h>
#include <string>
#include "../statemachine.hpp"
#include "../subline.hpp"

#define PARSER_STATE_CLASS_DECL(NAME, ID)                                \
    class NAME : public StateType                                        \
    {                                                                    \
        public:                                                          \
        static const uint8_t id = (ID);                                  \
        virtual std::unique_ptr<StateType> transition(ass_res_t &value); \
        void output(ass_res_t &value);                     \
    }

namespace ass2srt::ass::parserstate {
    /**
     * Intermediate parsing result
     */
    struct ass_res_t
    {
        std::istream &istream;
        subtitles_t &result;
        std::string token;
        bool eof;

        ass_res_t(std::istream &istream_, subtitles_t &result_);

        ass_res_t& operator=(ass_res_t& value);
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
        // STYLES,
        // ...
        
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
     * Final state
     */
    typedef statemachine::FinalState<ass_res_t> FinalState;
}

#endif

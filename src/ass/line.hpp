#ifndef _ASS2SRT_ASS_LINE_H
#define _ASS2SRT_ASS_LINE_H

#include <list>
#include <set>
#include <stdint.h>
#include <string>
#include "field.hpp"

namespace ass2srt::ass::line {
    /**
     * Possible line types
     */
    enum LineType
    {
        PLAY_RES_Y = 0,
        FORMAT,
        STYLE,
        DIALOGUE,
        UNKNOWN = UINT8_MAX
    };

    /**
     * Line info
     */
    struct line_type_t
    {
        /**
         * Definition in the file as a string
         */
        const char *definition;

        /**
         * Unique field id
         */
        const LineType id;
    };

    /**
     * Available fields
     */
    const line_type_t line_type_list[] = {
        { "PlayResY", PLAY_RES_Y },
        { "Format", FORMAT },
        { "Style", STYLE },
        { "Dialogue", DIALOGUE },
    };

    /**
     * Parse the line type definition
     */
    LineType parse_type(std::string &);

    /**
     * Get line value (w/o type)
     */
    std::string get_line_value(std::string &);

    /**
     * Parse section format line
     */
    std::list<field::FieldType> parse_format_declaration(std::string &, std::set<std::string>);
}

#endif

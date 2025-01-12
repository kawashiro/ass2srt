#ifndef ASS2SRT_ASS_LINE_H
#define ASS2SRT_ASS_LINE_H

#include "field.hpp"
#include <array>
#include <cstdint>
#include <set>
#include <string>
#include <vector>

namespace ass2srt::ass::line {
/**
 * Possible line types
 */
enum LineType : std::uint8_t {
    PLAY_RES_Y = 0,
    FORMAT = 1,
    STYLE = 2,
    DIALOGUE = 3,
    UNKNOWN = UINT8_MAX
};

/**
 * Line info
 */
struct line_type_t {
    /**
     * Definition in the file as a string
     */
    const char* definition;

    /**
     * Unique field id
     */
    LineType id;
};

/**
 * Available fields
 */
const std::array<line_type_t, 4> line_type_list = { {
    { .definition = "PlayResY", .id = PLAY_RES_Y },
    { .definition = "Format", .id = FORMAT },
    { .definition = "Style", .id = STYLE },
    { .definition = "Dialogue", .id = DIALOGUE },
} };

/**
 * Parse the line type definition
 */
auto parse_type(const std::string&) -> LineType;

/**
 * Get line value (w/o type)
 */
auto get_line_value(const std::string&) -> std::string;

/**
 * Parse section format line
 */
auto parse_format_declaration(const std::string&, const std::set<std::string>&) -> std::vector<field::FieldType>;
}

#endif

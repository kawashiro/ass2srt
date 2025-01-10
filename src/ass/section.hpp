#ifndef ASS2SRT_ASS_SECTION_H
#define ASS2SRT_ASS_SECTION_H

#include <array>
#include <cstdint>
#include <string>

namespace ass2srt::ass::section {
/**
 * Possible file sections
 */
enum Section : std::uint8_t {
    SCRIPT_INFO = 0,
    STYLES = 1,
    EVENTS = 2,
    UNKNOWN = UINT8_MAX
};

/**
 * Section info
 */
struct section_t {
    /**
     * Definition in the file as a string [Some Definition] => "Some Definition"
     */
    const char* definition;

    /**
     * Unique section id
     */
    const Section id;
};

/**
 * Available sections
 */
const std::array<section_t, 4> sections_list = { {
    { .definition = "Script Info", .id = SCRIPT_INFO },
    { .definition = "V4 Styles", .id = STYLES },
    { .definition = "V4+ Styles", .id = STYLES },
    { .definition = "Events", .id = EVENTS },
} };

/**
 * Parse the section definition
 */
auto parse(const std::string&) -> Section;
}

#endif

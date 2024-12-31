#ifndef _ASS2SRT_ASS_SECTION_H
#define _ASS2SRT_ASS_SECTION_H

#include <stdint.h>
#include <string>

namespace ass2srt::ass::section {
    /**
     * Possible file sections
     */
    enum Section
    {
        SCRIPT_INFO = 0,
        STYLES,
        EVENTS,
        UNKNOWN = UINT8_MAX
    };

    /**
     * Section info
     */
    struct section_t
    {
        /**
         * Definition in the file as a string [Some Definition] => "Some Definition"
         */
        const char *definition;

        /**
         * Unique section id
         */
        const Section id;
    };

    /**
     * Available sections
     */
    const section_t sections_list[] = {
        { "Script Info", SCRIPT_INFO },
        { "V4 Styles", STYLES },
        { "V4+ Styles", STYLES },
        { "Events", EVENTS },
    };

    /**
     * Parse the section definition
     */
    Section parse(const std::string &);
}

#endif

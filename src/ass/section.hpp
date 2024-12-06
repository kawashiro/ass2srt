#ifndef _ASS2SRT_ASS_SECTION_H
#define _ASS2SRT_ASS_SECTION_H

#include <stdint.h>
#include <string>

namespace ass2srt { namespace ass { namespace section {
    /**
     * Possible file sections
     */
    enum Section
    {
        ROOT = 0,

        SCRIPT_INFO,
        STYLES,
        EVENTS,
        // ...
        
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
    Section parse(std::string &);
} } }

#endif

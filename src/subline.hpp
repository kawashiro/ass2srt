#ifndef _ASS2SRT_SUBLINE_H
#define _ASS2SRT_SUBLINE_H

#include <list>
#include <string>

namespace ass2srt {
    /**
     * One smallest part uf subline
     */
    struct subline_part {
        /**
         * Vertical position: 0 - top, 1 - bottom
         */
        float v_pos;

        /**
         * Inner text
         */
        std::string text;
    };

    /**
     * Vertically aligned subtitle line
     */
    struct subline {
        /**
         * Start time
         */
        long start_milis;

        /**
         * End time
         */
        long end_milis;
        
        /**
         * Ordered text parts
         */
        std::list<subline_part> parts;
    };

    typedef std::list<subline> subtitles_t;
}

#endif
#ifndef _ASS2SRT_ASS_H
#define _ASS2SRT_ASS_H

#include <istream>
#include "subline.hpp"

namespace ass2srt::ass {
    /**
     * Parse input ASS data
     */
    subtitles_t parse_ass_stream(std::istream &);
}

#endif

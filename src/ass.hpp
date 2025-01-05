#ifndef _ASS2SRT_ASS_H
#define _ASS2SRT_ASS_H

#include <istream>
#include <set>
#include <string>
#include "subline.hpp"

namespace ass2srt::ass {
    /**
     * Parse input ASS data
     */
    subtitles_t parse_ass_stream(std::istream &, const std::set<std::string> &, const std::set<std::string> &, const bool);

    /**
     * Parse input ASS data with default args
     */
    subtitles_t parse_ass_stream(std::istream &);
}

#endif

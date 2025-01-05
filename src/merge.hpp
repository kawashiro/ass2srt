#ifndef _ASS2SRT_MERGE_H
#define _ASS2SRT_MERGE_H

#include "subline.hpp"

namespace ass2srt::merge {
    /**
     * Merge all subtitles into one part with no intersection by the time
     */
    subtitles_t merge_subtitles_parts(const subtitles_t &);
}

#endif

#ifndef ASS2SRT_MERGE_H
#define ASS2SRT_MERGE_H

#include "subline.hpp"

namespace ass2srt::merge {
/**
 * Merge all subtitles into one part with no intersection by the time
 */
auto merge_subtitles_parts(const subtitles_t&) -> subtitles_t;
}

#endif

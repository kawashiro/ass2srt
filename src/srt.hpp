#ifndef ASS2SRT_SRT_H
#define ASS2SRT_SRT_H

#include "subline.hpp"
#include <ostream>

namespace ass2srt::srt {
/**
 * Write SRT data
 */
void write_srt_stream(const subtitles_t&, std::ostream&);
}

#endif

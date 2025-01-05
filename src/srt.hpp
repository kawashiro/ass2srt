#ifndef _ASS2SRT_SRT_H
#define _ASS2SRT_SRT_H

#include <ostream>
#include "subline.hpp"

namespace ass2srt::srt {
    /**
     * Write SRT data
     */
    void write_srt_stream(const subtitles_t &, std::ostream &);
}

#endif

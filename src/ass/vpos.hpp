#ifndef _ASS2SRT_ASS_VPOS_H
#define _ASS2SRT_ASS_VPOS_H

#include "field.hpp"

namespace ass2srt::ass::vpos {
    /**
     * Calculate vertical position for all the styles applied
     */
    float calculate_vpos(const int, const int, const field::styles_spec_t &, const field::styles_spec_t &);
}

#endif

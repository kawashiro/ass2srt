#ifndef _ASS2SRT_ASS_VPOS_H
#define _ASS2SRT_ASS_VPOS_H

#include "field.hpp"

namespace ass2srt::ass::vpos {
    /**
     * Calculate vertical position for all the styles applied
     */
    float calculate_vpos(int v_size, int line_margin_v, field::styles_spec_t &line_style, field::styles_spec_t &inline_style);
}

#endif

#ifndef ASS2SRT_ASS_VPOS_H
#define ASS2SRT_ASS_VPOS_H

#include "field.hpp"

namespace ass2srt::ass::vpos {
/**
 * Calculate vertical position for all the styles applied
 */
auto calculate_vpos(int, int, const field::styles_spec_t&, const field::styles_spec_t&) -> float;
}

#endif

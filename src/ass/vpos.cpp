#include <cmath>
#include <stdexcept>
#include <stdint.h>
#include "field.hpp"
#include "vpos.hpp"
#include "../strutils.hpp"

using namespace ass2srt::ass;

float vpos::calculate_vpos(const int v_size, const int line_margin_v, const field::styles_spec_t &line_style, const field::styles_spec_t &inline_style)
{
    int y_pos;
    if (inline_style.explicit_y_pos >= 0) {
        y_pos = inline_style.explicit_y_pos;
    } else {
        int effective_margin = inline_style.margin_v >= 0 ? inline_style.margin_v :
            line_margin_v >= 0 ? line_margin_v : line_style.margin_v;
        uint8_t effective_alignment = inline_style.alignment > 0 ? inline_style.alignment : line_style.alignment;
        if (ALIGN_IS_BOTTOM(effective_alignment)) {
            y_pos = effective_margin;
        } else if (ALIGN_IS_TOP(effective_alignment)) {
            y_pos = v_size - effective_margin;
        } else if (ALIGN_IS_MIDDLE(effective_alignment)) {
            y_pos = std::round(v_size / 2.0);
        } else {
            throw std::invalid_argument(strutils::format("Invalid alignment value %d", effective_alignment));
        }
    }

    return static_cast<float>(y_pos) / v_size;
}

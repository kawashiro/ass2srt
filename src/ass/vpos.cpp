// #include <cmath>
#include <cstdint>
#include <stdexcept>
#include "field.hpp"
#include "vpos.hpp"
#include "../strutils.hpp"

using namespace ass2srt::ass;

float vpos::calculate_vpos(const int v_size, const int line_margin_v, const field::styles_spec_t &line_style, const field::styles_spec_t &inline_style)
{
    uint8_t effective_alignment = inline_style.alignment > 0 ? inline_style.alignment : line_style.alignment;
    if (!ALIGN_VALID(effective_alignment)) {
        throw std::invalid_argument(strutils::format("Invalid alignment value %d", effective_alignment));
    }
    float v_size_f = static_cast<float>(v_size);

    float result;
    if (v_size_f > 0) {
        int effective_margin =
            inline_style.margin_v >= 0 ? inline_style.margin_v :
            line_margin_v > 0 ? line_margin_v :
            line_style.margin_v;
        if (inline_style.explicit_y_pos >= 0) {
            result = 1.0 - inline_style.explicit_y_pos / v_size_f;
        } else if (ALIGN_IS_BOTTOM(effective_alignment)) {
            result = effective_margin / v_size_f;
        } else if (ALIGN_IS_TOP(effective_alignment)) {
            result = 1.0 - effective_margin / v_size_f;
        } else { // ALIGN_IS_MIDDLE
            result = 0.5;
        }
    } else {
        if (ALIGN_IS_BOTTOM(effective_alignment)) {
            result = 0.0;
        } else if (ALIGN_IS_TOP(effective_alignment)) {
            result = 1.0;
        } else { // ALIGN_IS_MIDDLE
            result = 0.5;
        }
    }

    return result;
}

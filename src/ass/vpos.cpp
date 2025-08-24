#include "vpos.hpp"
#include "../strutils.hpp"
#include "field.hpp"
#include <cmath>
#include <cstdint>
#include <stdexcept>

using namespace ass2srt::ass;

auto vpos::calculate_vpos(const int v_size, const int line_margin_v, const field::styles_spec_t& line_style, const field::styles_spec_t& inline_style) -> float
{
    const uint8_t effective_alignment = inline_style.alignment > 0 ? inline_style.alignment : line_style.alignment;
    if (!ALIGN_VALID(effective_alignment)) {
        throw std::invalid_argument(strutils::format("Invalid alignment value %d", effective_alignment));
    }
    const auto v_size_f = static_cast<float>(v_size);

    float result = NAN;
    if (v_size_f > 0) {
        const int event_margin = inline_style.margin_v >= 0 ? inline_style.margin_v : line_margin_v;
        const auto effective_margin = static_cast<float>(event_margin > 0 ? event_margin : line_style.margin_v);
        if (inline_style.explicit_y_pos >= 0) {
            result = 1.0F - inline_style.explicit_y_pos / v_size_f;
        } else if (ALIGN_IS_BOTTOM(effective_alignment)) {
            result = effective_margin / v_size_f;
        } else if (ALIGN_IS_TOP(effective_alignment)) {
            result = 1.0F - effective_margin / v_size_f;
        } else { // ALIGN_IS_MIDDLE
            result = 0.5F;
        }
    } else {
        if (ALIGN_IS_BOTTOM(effective_alignment)) {
            result = 0.0F;
        } else if (ALIGN_IS_TOP(effective_alignment)) {
            result = 1.0F;
        } else { // ALIGN_IS_MIDDLE
            result = 0.5F;
        }
    }

    return result;
}

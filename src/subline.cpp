#include "subline.hpp"
#include <cmath>
#include <limits>

using namespace ass2srt;

auto subline_part::operator==(const subline_part& other) const -> bool
{
    return std::fabs(this->v_pos - other.v_pos) < std::numeric_limits<float>::epsilon() && this->text == other.text;
}

auto subline::operator==(const subline& other) const -> bool
{
    return this->start_milis == other.start_milis
        && this->end_milis == other.end_milis
        && this->parts == other.parts;
}

#include "subline.hpp"
#include "mathutils.hpp"
#include <cmath>

using namespace ass2srt;

auto subline_part::operator==(const subline_part& other) const -> bool
{
    return mathutils::is_close(this->v_pos, other.v_pos) && this->text == other.text;
}

auto subline::operator==(const subline& other) const -> bool
{
    return this->start_milis == other.start_milis
        && this->end_milis == other.end_milis
        && this->parts == other.parts;
}

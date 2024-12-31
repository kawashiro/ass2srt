#include <cmath>
#include <limits>
#include "subline.hpp"

using namespace ass2srt;

bool subline_part::operator ==(const subline_part &other) const
{
    return std::fabs(this->v_pos - other.v_pos) < std::numeric_limits<float>::epsilon() && this->text == other.text;
}

bool subline::operator ==(const subline &other) const
{
    return this->start_milis == other.start_milis
        && this->end_milis == other.end_milis
        && this->parts == other.parts;
}

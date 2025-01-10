#ifndef ASS2SRT_ASS_H
#define ASS2SRT_ASS_H

#include "subline.hpp"
#include <istream>
#include <set>
#include <string>

namespace ass2srt::ass {
/**
 * Parse input ASS data
 */
auto parse_ass_stream(std::istream&, const std::set<std::string>&, const std::set<std::string>&, bool) -> subtitles_t;

/**
 * Parse input ASS data with default args
 */
auto parse_ass_stream(std::istream&) -> subtitles_t;
}

#endif

#ifndef ASS2SRT_SUBLINE_H
#define ASS2SRT_SUBLINE_H

#include <list>
#include <string>
#include <vector>

namespace ass2srt {
/**
 * One smallest part uf subline
 */
struct subline_part {
    /**
     * Vertical position: 0 - top, 1 - bottom
     */
    float v_pos;

    /**
     * Horizontal position order
     */
    int x_order;

    /**
     * Inner text
     */
    std::string text;

    /**
     * Compare parts
     */
    auto operator==(const subline_part&) const -> bool;
};

/**
 * Vertically aligned subtitle line
 */
struct subline {
    /**
     * Start time
     */
    long start_milis;

    /**
     * End time
     */
    long end_milis;

    /**
     * Ordered text parts
     */
    std::vector<subline_part> parts;

    /**
     * Compare lines
     */
    auto operator==(const subline&) const -> bool;
};

using subtitles_t = std::list<subline>;
}

#endif

#include "merge.hpp"
#include "mathutils.hpp"
#include "subline.hpp"
#include <algorithm>
#include <cmath>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace ass2srt;

namespace {
struct subline_part_ext_t {
    long original_start;
    float v_pos;
    int x_order;
    std::string text;
};

class NonIntersectedSubtitlesList {
public:
    struct time_key {
        long start;
        long end;

        auto operator<(const time_key& other) const -> bool
        {
            if (this->start == other.start) {
                return this->end < other.end;
            }
            return this->start < other.start;
        }
    };

    using subs_vec = std::vector<subline_part_ext_t>;
    using iterator = std::map<time_key, subs_vec>::iterator;
    using const_iterator = std::map<time_key, subs_vec>::const_iterator;
    using cls = NonIntersectedSubtitlesList;

    explicit NonIntersectedSubtitlesList()
        : time_map({})
    {
    }

    NonIntersectedSubtitlesList(const NonIntersectedSubtitlesList&) = delete;
    NonIntersectedSubtitlesList(NonIntersectedSubtitlesList&&) = delete;
    auto operator=(const NonIntersectedSubtitlesList&) -> NonIntersectedSubtitlesList& = delete;
    auto operator=(NonIntersectedSubtitlesList&&) -> NonIntersectedSubtitlesList& = delete;
    ~NonIntersectedSubtitlesList() = default;

    void append(const long start, const long end, const subline_part& part)
    {
        time_key curr_key { .start = start, .end = end };
        const subline_part_ext_t part_ext { .original_start = start, .v_pos = part.v_pos, .x_order = part.x_order, .text = part.text };

        const subs_vec curr_parts = this->append0(curr_key.start, curr_key.end, { part_ext });

        curr_key.start = this->merge_left(curr_key, curr_parts);
        this->merge_right(curr_key, curr_parts);
    }

    auto begin() -> iterator
    {
        return this->time_map.begin();
    }

    [[nodiscard]] auto begin() const -> const_iterator
    {
        return this->time_map.cbegin();
    }

    auto end() -> iterator
    {
        return this->time_map.end();
    }

    [[nodiscard]] auto end() const -> const_iterator
    {
        return this->time_map.cend();
    }

    [[nodiscard]] auto empty() const -> bool
    {
        return this->time_map.empty();
    }

private:
    std::map<time_key, subs_vec> time_map;

    /**
     * Try to find something intersecting on the left
     * Returns the left border (start) of the actual current part
     */
    auto merge_left(const time_key& curr_key, const subs_vec& curr_parts) -> long
    {
        auto left_part_it = this->time_map.find(curr_key);
        if (left_part_it == this->time_map.begin()) {
            return curr_key.start;
        }
        left_part_it--;

        const subs_vec left_parts = left_part_it->second;
        const time_key left_key = left_part_it->first;
        if (left_key.end <= curr_key.start) {
            return curr_key.start;
        }

        this->time_map.erase(left_part_it);
        this->time_map.erase(curr_key);

        this->append0(left_key.start, curr_key.start, left_parts);
        this->append0(curr_key.start, std::min(left_key.end, curr_key.end), cls::concat(left_parts, curr_parts));
        this->append0(left_key.end, curr_key.end, curr_parts);
        this->append0(curr_key.end, left_key.end, left_parts);

        return std::min(left_key.end, curr_key.end);
    }

    /**
     * Try to find something intersecting on the right
     */
    void merge_right(const time_key& curr_key, const subs_vec& curr_parts)
    {
        time_key effective_curr_key(curr_key.start, curr_key.end);
        for (;;) {
            auto curr_part_it = this->time_map.find(effective_curr_key);
            if (curr_part_it == this->time_map.end()) {
                break;
            }
            auto right_part_it = std::next(curr_part_it);
            if (right_part_it == this->time_map.end()) {
                break;
            }

            const subs_vec right_parts = right_part_it->second;
            const time_key right_key = right_part_it->first;
            if (right_key.start >= effective_curr_key.end) {
                break;
            }

            this->time_map.erase(effective_curr_key);
            this->time_map.erase(right_part_it);
            this->append0(effective_curr_key.start, right_key.start, curr_parts);
            this->append0(right_key.start, std::min(right_key.end, effective_curr_key.end), cls::concat(right_parts, curr_parts));

            if (right_key.end < effective_curr_key.end) {
                this->append0(right_key.end, effective_curr_key.end, curr_parts);
                effective_curr_key.start = right_key.end;
            } else {
                this->append0(effective_curr_key.end, right_key.end, right_parts);
                effective_curr_key.start = effective_curr_key.end;
            }
        }
    }

    auto append0(const long start, const long end, const subs_vec& parts) -> subs_vec
    {
        if (start >= end || parts.empty()) {
            return {};
        }

        const time_key curr_key(start, end);
        auto curr_part_it = this->time_map.find(curr_key);
        if (curr_part_it == this->time_map.end()) {
            this->time_map[curr_key] = parts;
            return parts;
        }

        subs_vec merged_parts = cls::concat(curr_part_it->second, parts);
        this->time_map[curr_key] = merged_parts;
        return merged_parts;
    }

    static auto concat(const subs_vec& first, const subs_vec& second) -> subs_vec
    {
        subs_vec res(first);
        for (const auto& to_add : second) {
            res.push_back(to_add);
        }
        return res;
    }
};

auto merge_text(std::vector<subline_part_ext_t>& parts) -> std::string
{
    if (parts.empty()) {
        return "";
    }

    std::vector<subline_part_ext_t> parts_copy;
    std::set<std::string> met;
    for (const auto& part : parts) {
        if (met.contains(part.text)) {
            continue;
        }
        parts_copy.push_back(part);
        met.insert(part.text);
    }

    std::ranges::sort(
        parts_copy,
        [](subline_part_ext_t& first, subline_part_ext_t& second) {
            if (mathutils::is_close(first.v_pos, second.v_pos)) {
                if (first.original_start < second.original_start) {
                    return true;
                }
                return first.x_order < second.x_order;
            }
            return first.v_pos > second.v_pos;
        });

    std::string result = parts_copy[0].text;
    float prev_vpos = parts_copy[0].v_pos;
    for (auto it = std::next(parts_copy.begin()); it != parts_copy.end(); ++it) {
        if (mathutils::is_close(prev_vpos, it->v_pos)) {
            result += it->text;
        } else {
            result += "\n" + it->text;
        }
        prev_vpos = it->v_pos;
    }

    return result;
}
}

auto merge::merge_subtitles_parts(const subtitles_t& input) -> subtitles_t
{
    NonIntersectedSubtitlesList subtitles_by_time_breaks;
    for (const auto& line : input) {
        for (const auto& part : line.parts) {
            if (part.text.empty()) {
                continue;
            }
            subtitles_by_time_breaks.append(line.start_milis, line.end_milis, part);
        }
    }

    subtitles_t result;
    if (subtitles_by_time_breaks.empty()) {
        return result;
    }

    std::string prev_text;
    long prev_end = -1;
    for (auto merged_subtitles : subtitles_by_time_breaks) {
        const std::string merged_text = merge_text(merged_subtitles.second);
        if (prev_end == merged_subtitles.first.start && merged_text == prev_text) {
            result.back().end_milis = merged_subtitles.first.end;
        } else {
            const subline_part part { .v_pos = 0.0, .x_order = 0, .text = merged_text };
            const subline merged_line { .start_milis = merged_subtitles.first.start, .end_milis = merged_subtitles.first.end, .parts = { part } };
            result.push_back(merged_line);
            prev_text = merged_text;
        }
        prev_end = merged_subtitles.first.end;
    }

    return result;
}

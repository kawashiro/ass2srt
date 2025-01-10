#include "merge.hpp"
#include "subline.hpp"
#include <algorithm>
#include <cmath>
#include <iterator>
#include <limits>
#include <map>
#include <string>
#include <vector>

using namespace ass2srt;

namespace {
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

    using subs_vec = std::vector<subline_part>;
    using iterator = std::map<time_key, subs_vec>::iterator;
    using const_iterator = std::map<time_key, subs_vec>::const_iterator;

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
        const subs_vec curr_parts = this->append0(curr_key.start, curr_key.end, { part });

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
        this->append0(curr_key.start, std::min(left_key.end, curr_key.end), NonIntersectedSubtitlesList::concat(left_parts, curr_parts));
        this->append0(left_key.end, curr_key.end, curr_parts);
        this->append0(curr_key.end, left_key.end, left_parts);

        return std::min(left_key.end, curr_key.end);
    }

    /**
     * Try to find something intersecting on the right
     */
    void merge_right(const time_key& curr_key, const subs_vec& curr_parts)
    {
        struct to_append_t {
            long start;
            long end;
            subs_vec parts;
        };
        std::vector<time_key> to_erase {};
        std::vector<to_append_t> to_append {};

        time_key effective_curr_key(curr_key.start, curr_key.end);
        for (auto right_part_it = std::next(this->time_map.find(effective_curr_key)); right_part_it != this->time_map.end(); ++right_part_it) {
            const subs_vec right_parts = right_part_it->second;
            const time_key right_key = right_part_it->first;
            if (right_key.start >= effective_curr_key.end) {
                break;
            }

            to_erase.push_back(right_key);
            to_append.push_back({ effective_curr_key.start, right_key.start, curr_parts });
            to_append.push_back({ right_key.start, std::min(right_key.end, effective_curr_key.end), NonIntersectedSubtitlesList::concat(right_parts, curr_parts) });

            if (right_key.end < effective_curr_key.end) {
                to_append.push_back({ right_key.end, effective_curr_key.end, curr_parts });
                effective_curr_key.start = right_key.end;
            } else {
                to_append.push_back({ effective_curr_key.end, right_key.end, right_parts });
                effective_curr_key.start = effective_curr_key.end;
            }
        }
        if (!to_erase.empty()) {
            to_erase.push_back(curr_key);
        }

        for (auto erase : to_erase) {
            this->time_map.erase(erase);
        }
        for (const auto& append : to_append) {
            this->append0(append.start, append.end, append.parts);
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

        subs_vec merged_parts = NonIntersectedSubtitlesList::concat(curr_part_it->second, parts);
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

auto merge_text(std::vector<subline_part>& parts) -> std::string
{
    if (parts.empty()) {
        return "";
    }

    auto parts_copy = parts;
    std::ranges::sort(
        parts_copy,
        [](subline_part& first, subline_part& second) {
            if (std::fabs(first.v_pos - second.v_pos) < std::numeric_limits<float>::epsilon()) {
                return first.x_order < second.x_order;
            }
            return first.v_pos > second.v_pos;
        });

    std::string result = parts_copy[0].text;
    float prev_vpos = parts_copy[0].v_pos;
    for (auto it = std::next(parts_copy.begin()); it != parts_copy.end(); ++it) {
        if (std::fabs(prev_vpos - it->v_pos) < std::numeric_limits<float>::epsilon()) {
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
    for (auto merged_subtitles : subtitles_by_time_breaks) {
        const std::string merged_text = merge_text(merged_subtitles.second);
        const subline_part part { .v_pos = 0.0, .x_order = 0, .text = merged_text };
        const subline merged_line { .start_milis = merged_subtitles.first.start, .end_milis = merged_subtitles.first.end, .parts = { part } };
        result.push_back(merged_line);
    }

    return result;
}

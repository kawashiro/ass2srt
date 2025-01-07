#include <algorithm>
#include <cmath>
#include <iterator>
#include <limits>
#include <map>
#include <vector>
#include "merge.hpp"
#include "subline.hpp"

using namespace ass2srt;

class NonIntersectedSubtitlesList
{
    public:
    struct time_key
    {
        long start;
        long end;

        bool operator <(const time_key &other) const
        {
            if (this->start < other.start) {
                return true;
            }
            if (this->start > other.start) {
                return false;
            }
            return this->end < other.end;
        }
    };

    typedef std::vector<subline_part> subs_vec;
    typedef std::map<time_key, subs_vec>::iterator iterator;
    typedef std::map<time_key, subs_vec>::const_iterator const_iterator;

    explicit NonIntersectedSubtitlesList(): time_map({})
    {
    }

    NonIntersectedSubtitlesList(const NonIntersectedSubtitlesList &) = delete;
    NonIntersectedSubtitlesList &operator =(const NonIntersectedSubtitlesList &) = delete;

    void append(const long start, const long end, const subline_part &part)
    {
        time_key curr_key {start, end};
        subs_vec curr_parts = this->append0(curr_key.start, curr_key.end, {part});

        curr_key.start = this->merge_left(curr_key, curr_parts);
        this->merge_right(curr_key, curr_parts);
    }

    iterator begin()
    {
        return this->time_map.begin();
    }

    const_iterator begin() const
    {
        return this->time_map.cbegin();
    }

    iterator end()
    {
        return this->time_map.end();
    }

    const_iterator end() const
    {
        return this->time_map.cend();
    }

    private:
    std::map<time_key, subs_vec> time_map;

    /**
     * Try to find something intersecting on the left
     * Returns the left border (start) of the actual current part
     */
    long merge_left(const time_key &curr_key, const subs_vec &curr_parts)
    {
        auto left_part_it = this->time_map.find(curr_key);
        if (left_part_it == this->time_map.begin()) {
            return curr_key.start;
        }
        left_part_it--;

        subs_vec left_parts = left_part_it->second;
        time_key left_key = left_part_it->first;
        if (left_key.end <= curr_key.start) {
            return curr_key.start;
        }

        this->time_map.erase(left_part_it);
        this->time_map.erase(curr_key);

        this->append0(left_key.start, curr_key.start, left_parts);
        this->append0(curr_key.start, std::min(left_key.end, curr_key.end), this->concat(left_parts, curr_parts));
        this->append0(left_key.end, curr_key.end, curr_parts);
        this->append0(curr_key.end, left_key.end, left_parts);

        return std::min(left_key.end, curr_key.end);
    }

    /**
     * Try to find something intersecting on the right
     */
    void merge_right(const time_key &curr_key, const subs_vec &curr_parts)
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
            subs_vec right_parts = right_part_it->second;
            time_key right_key = right_part_it->first;
            if (right_key.start >= effective_curr_key.end) {
                break;
            }

            to_erase.push_back(right_key);
            to_append.push_back({effective_curr_key.start, right_key.start, curr_parts});
            to_append.push_back({right_key.start, std::min(right_key.end, effective_curr_key.end), this->concat(right_parts, curr_parts)});

            if (right_key.end < effective_curr_key.end) {
                to_append.push_back({right_key.end, effective_curr_key.end, curr_parts});
                effective_curr_key.start = right_key.end;
            } else {
                to_append.push_back({effective_curr_key.end, right_key.end, right_parts});
                effective_curr_key.start = effective_curr_key.end;
            }
        }
        if (!to_erase.empty()) {
            to_erase.push_back(curr_key);
        }

        for (auto erase : to_erase) {
            this->time_map.erase(erase);
        }
        for (auto append : to_append) {
            this->append0(append.start, append.end, append.parts);
        }
    }

    subs_vec append0(const long start, const long end, const subs_vec &parts)
    {
        if (start >= end || parts.empty()) {
            return {};
        }

        time_key curr_key(start, end);
        auto curr_part_it = this->time_map.find(curr_key);
        if (curr_part_it == this->time_map.end()) {
            this->time_map[curr_key] = parts;
            return parts;
        }

        subs_vec merged_parts = this->concat(curr_part_it->second, parts);
        this->time_map[curr_key] = merged_parts;
        return merged_parts;
    }

    static subs_vec concat(const subs_vec &a, const subs_vec &b)
    {
        subs_vec res(a);
        for (auto sub_b : b) {
            res.push_back(sub_b);
        }
        return res;
    }
};

static std::string merge_text(std::vector<subline_part> &parts)
{
    if (parts.empty()) {
        return "";
    }

    auto parts_copy = parts;
    std::sort(
        parts_copy.begin(),
        parts_copy.end(),
        [](subline_part &a, subline_part &b) {
            if (std::fabs(a.v_pos - b.v_pos) < std::numeric_limits<float>::epsilon()) {
                return a.x_order < b.x_order;
            }
            return a.v_pos > b.v_pos;
        }
    );

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

subtitles_t merge::merge_subtitles_parts(const subtitles_t &input)
{
    NonIntersectedSubtitlesList subtitles_by_time_breaks;
    for (auto line : input) {
        for (auto part : line.parts) {
            if (part.text.empty()) {
                continue;
            }
            subtitles_by_time_breaks.append(line.start_milis, line.end_milis, part);
        }
    }

    subtitles_t result;
    for (auto merged_subtitles : subtitles_by_time_breaks) {
        std::string merged_text = merge_text(merged_subtitles.second);
        subline_part part {0.0, 0, merged_text};
        subline merged_line {merged_subtitles.first.start, merged_subtitles.first.end, {part}};
        result.push_back(merged_line);
    }

    return result;
}

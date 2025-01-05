#include <algorithm>
#include <iterator>
#include <map>
#include <vector>
#include "merge.hpp"
#include "subline.hpp"

using namespace ass2srt;

struct merged_line_t
{
    long start_millis;
    long end_millis;
    std::vector<subline_part> parts;
};

struct merged_lines_t : std::map<long, merged_line_t>
{
    void insert(const long start, const long end, const subline_part &part)
    {
        merged_line_t line = (*this)[start];
        line.start_millis = start;
        line.end_millis = end;
        line.parts.push_back(part);
        insert0(line);

        auto current_line = line;
        std::list<merged_line_t> fully_shadowed_lines;
        for (auto it = ++this->find(current_line.start_millis); it != this->end(); ++it) {
            if (it->second.start_millis < current_line.start_millis || it->second.end_millis > current_line.end_millis) {
                continue;
            }
            fully_shadowed_lines.push_back(it->second);
        }

        auto prev_line_it = this->find(current_line.start_millis);
        if (prev_line_it != this->begin()) {
            --prev_line_it;
            auto prev_line = prev_line_it->second;
            auto prev_line_end_orig = prev_line.end_millis;
            if (prev_line_end_orig > current_line.start_millis) {
                for (auto prev_part : prev_line.parts) {
                    line.parts.push_back(prev_part);
                }
                line.end_millis = std::min(line.end_millis, prev_line_end_orig);
                insert0(line);
                prev_line.end_millis = current_line.start_millis;
                insert0(prev_line);
            }

            if (prev_line_end_orig > current_line.end_millis) {
                merged_line_t post_line;
                post_line.start_millis = current_line.end_millis;
                post_line.end_millis = prev_line_end_orig;
                post_line.parts = prev_line.parts;
                insert0(post_line);
            } else if (current_line.start_millis < prev_line_end_orig) {
                merged_line_t post_line;
                post_line.start_millis = prev_line_end_orig;
                post_line.end_millis = current_line.end_millis;
                post_line.parts = current_line.parts;
                insert0(post_line);
            }

        } else {
            ++prev_line_it;
            if (prev_line_it != this->end()) {
                auto prev_line = prev_line_it->second;
                auto prev_line_end_orig = prev_line.end_millis;
                auto prev_line_parts_orig = prev_line.parts;
                if (prev_line.start_millis < current_line.end_millis) {
                    prev_line.end_millis = current_line.end_millis;
                    for (auto prev_part : current_line.parts) {
                        prev_line.parts.push_back(prev_part);
                    }
                    insert0(prev_line);
                    line.end_millis = prev_line.start_millis;
                    insert0(line);
                    merged_line_t post_line;
                    post_line.start_millis = current_line.end_millis;
                    post_line.end_millis = prev_line_end_orig;
                    post_line.parts = prev_line_parts_orig;
                    insert0(post_line);
                }
            }
        }

        for (auto to_reinsert : fully_shadowed_lines) {
            this->erase(to_reinsert.start_millis);
        }
        for (auto to_reinsert : fully_shadowed_lines) {
            for (auto part : to_reinsert.parts) {
                insert(to_reinsert.start_millis, to_reinsert.end_millis, part);
            }
        }
    }

    private:
    inline void insert0(merged_line_t &value)
    {
        (*this)[value.start_millis] = value;
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
            return a.v_pos > b.v_pos;
        }
    );

    std::string result = parts_copy[0].text;
    for (auto it = std::next(parts_copy.begin()); it != parts_copy.end(); ++it) {
        result += "\n" + it->text;
    }

    return result;
}

subtitles_t merge::merge_subtitles_parts(const subtitles_t &input)
{
    merged_lines_t subtitles_by_time_breaks;
    for (auto line : input) {
        for (auto part : line.parts) {
            if (part.text.empty()) {
                continue;
            }
            subtitles_by_time_breaks.insert(line.start_milis, line.end_milis, part);
        }
    }

    subtitles_t result;
    for (auto& [start_millis, line_by_time] : subtitles_by_time_breaks) {
        std::string merged_text = merge_text(line_by_time.parts);
        subline_part part {0.0, merged_text};
        subline merged_line {start_millis, line_by_time.end_millis, {part}};
        result.push_back(merged_line);
    }

    return result;
}

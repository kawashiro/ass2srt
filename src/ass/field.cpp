#include "field.hpp"
#include "../strutils.hpp"
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

using namespace ass2srt::ass;

#define FIELD_PARSE_IF_NEEDED(FLD, INP, OUT)       \
    do {                                           \
        auto field_proc_fn = this->parsers[(FLD)]; \
        if (field_proc_fn != nullptr) {            \
            auto result = field_proc_fn((INP));    \
            (OUT)[(FLD)] = result;                 \
        }                                          \
    } while (0)

namespace {
inline auto col_value(const std::string& input, const size_t pos) -> std::string
{
    return input.substr(pos, input.find(',', pos) - pos);
}

inline auto text_col_value(const std::string& input, const size_t pos) -> std::string
{
    return input.substr(pos, input.length() - pos);
}
}

field::styles_spec_t::styles_spec_t()
    : alignment(0)
    , margin_v(-1)
    , explicit_y_pos(-1)
    , is_drawing(false)
{
}

field::styles_spec_t::styles_spec_t(uint8_t alignment, int margin_v, float explicit_y_pos, bool is_drawing)
    : alignment(alignment)
    , margin_v(margin_v)
    , explicit_y_pos(explicit_y_pos)
    , is_drawing(is_drawing)
{
}

auto field::parse_type(const std::string& value) -> field::FieldType
{
    for (auto field : fields_list) {
        if (value == field.definition) {
            return field.id;
        }
    }

    return UNKNOWN;
}

auto field::parse_time_millis(const std::string& time_s) -> long
{
    const int millis_mult = 10;
    const int seconds_mult = 1000;
    const int minutes_mult = 60 * 1000;
    const int hours_mult = 60 * 60 * 1000;

    unsigned int hours = 0;
    unsigned int minutes = 0;
    unsigned int seconds = 0;
    unsigned int millis = 0;

    auto res = std::sscanf(time_s.c_str(), "%u:%u:%u.%u", &hours, &minutes, &seconds, &millis);
    if (res != 4) {
        throw std::invalid_argument("Invalid time value");
    }

    return (millis * millis_mult) + (seconds * seconds_mult) + (minutes * minutes_mult) + (hours * hours_mult);
}

auto field::parse_inline_style(const std::string& value) -> field::styles_spec_t
{
    field::styles_spec_t result;

    auto styles = strutils::split(value.substr(1, value.length() - 2), '\\');
    for (auto style_ptr = ++styles.begin(); style_ptr != styles.end(); ++style_ptr) {
        int drawing_flag = -1;
        if (std::sscanf(style_ptr->c_str(), "p%d", &drawing_flag) == 1) {
            result.is_drawing = drawing_flag > 0;
            continue;
        }

        int alignment = 0;
        bool alignment_parsed = false;
        for (const auto* alignment_format : { "a%d", "an%d" }) {
            if (std::sscanf(style_ptr->c_str(), alignment_format, &alignment) == 1) {
                if (!ALIGN_VALID(alignment)) {
                    throw std::invalid_argument(strutils::format("Invalid alignment %d", alignment));
                }
                result.alignment = alignment;
                alignment_parsed = true;
                break;
            }
        }
        if (alignment_parsed) {
            continue;
        }

        const size_t args_start = style_ptr->find('(');
        if (args_start >= style_ptr->length() - 1) {
            continue;
        }
        const size_t args_end = style_ptr->find(')');
        if (args_end != style_ptr->length() - 1) {
            continue;
        }

        auto fn_name_s = style_ptr->substr(0, args_start);
        if (fn_name_s == "pos") {
            auto fn_args_s = style_ptr->substr(args_start + 1, args_end - args_start - 1);
            auto args_list = strutils::split(fn_args_s, ',');
            if (args_list.size() != 2) {
                throw std::invalid_argument("Invalid arguments count for pos() function");
            }
            result.explicit_y_pos = std::stof(args_list[1]);

        } else if (fn_name_s == "move") {
            const size_t min_args_cnt = 4;
            const size_t max_args_cnt = 6;

            auto fn_args_s = style_ptr->substr(args_start + 1, args_end - args_start - 1);
            auto args_list = strutils::split(fn_args_s, ',');
            if (args_list.size() < min_args_cnt || args_list.size() > max_args_cnt) {
                throw std::invalid_argument("Invalid arguments count for move() function");
            }
            auto y_begin = std::stof(args_list[1]);
            auto y_end = std::stof(args_list[3]);
            result.explicit_y_pos = std::round((y_begin + y_end) / 2.0F);
        }
    }

    return result;
}

auto field::parse_plain_text(const std::string& value) -> std::string
{
    std::string out = value;
    strutils::replace_all(out, "\\N", "\n");
    out = strutils::dedup_eol(out);

    return out;
}

field::LineValuesParser::LineValuesParser()
    : result()
    , parsers()
    , dealloc()
{
    std::memset(&this->parsers, 0, sizeof(size_t) * (UNKNOWN + 1));
    std::memset(&this->result, 0, sizeof(size_t) * (UNKNOWN + 1));
    std::memset(&this->dealloc, 0, sizeof(size_t) * (UNKNOWN + 1));
}

void field::LineValuesParser::parse(const std::vector<field::FieldType>& format, const std::string& input_value)
{
    size_t str_pos = 0;
    for (auto field : format) {
        if (field == TEXT) {
            auto curr_value = text_col_value(input_value, str_pos);
            FIELD_PARSE_IF_NEEDED(field, curr_value, this->result);
            break; // Text is always the last field!
        }

        auto curr_value = col_value(input_value, str_pos);
        FIELD_PARSE_IF_NEEDED(field, curr_value, this->result);

        str_pos = input_value.find(',', str_pos) + 1;
    }
}

field::LineValuesParser::~LineValuesParser()
{
    for (size_t i = 0; i <= UNKNOWN; ++i) {
        if (this->result[i] == nullptr) {
            continue;
        }
        if (this->dealloc[i] == nullptr) {
            // TODO: Error?
            continue;
        }
        this->dealloc[i](this->result[i]);
        this->result[i] = nullptr;
    }
}

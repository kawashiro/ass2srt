#include <list>
#include <cmath>
#include <cstring>
#include <stdexcept>
#include <string>
#include "field.hpp"
#include "../strutils.hpp"

using namespace ass2srt::ass;

#define FIELD_PARSE_IF_NEEDED(FLD, INP, OUT)       \
    do {                                           \
        auto field_proc_fn = this->parsers[(FLD)]; \
        if (field_proc_fn != nullptr) {            \
            auto result = field_proc_fn((INP));    \
            (OUT)[(FLD)] = result;          \
        }                                          \
    } while (0)

static inline std::string col_value(std::string &input, size_t pos)
{
    return input.substr(pos, input.find(',', pos) - pos);
}

static inline std::string text_col_value(std::string &input, size_t pos)
{
    return input.substr(pos, input.length() - pos);
}

template<typename T>
static inline T get_list_el(std::list<T> &list, int pos)
{
    auto it = list.begin();
    for (int i = pos; i > 0; --i) {
        ++it;
    }
    return *it;
}

field::styles_spec_t::styles_spec_t():
    alignment(0),
    margin_v(-1),
    explicit_y_pos(-1)
{
}

field::styles_spec_t::styles_spec_t(uint8_t alignment, int margin_v, int explicit_y_pos)
{
    this->alignment = alignment;
    this->margin_v = margin_v;
    this->explicit_y_pos = explicit_y_pos;
}

field::styles_spec_t::styles_spec_t(const field::styles_spec_t &value)
{
    this->alignment = value.alignment;
    this->margin_v = value.margin_v;
    this->explicit_y_pos = value.explicit_y_pos;
}

field::styles_spec_t& field::styles_spec_t::operator =(const field::styles_spec_t &value)
{
    this->alignment = value.alignment;
    this->margin_v = value.margin_v;
    this->explicit_y_pos = value.explicit_y_pos;
    return *this;
}

field::FieldType field::parse_type(std::string &value)
{
    for (auto field : fields_list) {
        if (value == field.definition) {
            return field.id;
        }
    }

    return UNKNOWN;
}

long field::parse_time_millis(std::string &time_s)
{
    unsigned int hours;
    unsigned int minutes;
    unsigned int seconds;
    unsigned int millis;

    auto res = std::sscanf(time_s.c_str(), "%u:%u:%u.%u", &hours, &minutes, &seconds, &millis);
    if (res != 4) {
        throw std::invalid_argument("Invalid time value");
    }

    return millis * 10 + seconds * 1000 + minutes * 60 * 1000 + hours * 60 * 60 * 1000;
}

field::styles_spec_t field::parse_inline_style(std::string &value)
{
    field::styles_spec_t result;

    auto styles = strutils::split(value.substr(1, value.length() - 2), '\\');
    for (auto style_ptr = ++styles.begin(); style_ptr != styles.end(); ++style_ptr) {
        int alignment;
        if (std::sscanf(value.c_str(), "a%d", &alignment) == 1) {
            result.alignment = alignment;
            continue;
        }

        size_t args_start = style_ptr->find('(');
        if (args_start >= style_ptr->length() - 1) {
            continue;
        }
        size_t args_end = style_ptr->find(')');
        if (args_end != style_ptr->length() - 1) {
            continue;
        }

        auto fn_name_s = style_ptr->substr(0, args_start);
        if (fn_name_s == "pos") {
            auto fn_args_s = style_ptr->substr(args_start + 1, args_end - args_start - 1);
            auto args_list = strutils::split(fn_args_s, ',');
            if (args_list.size() != 2) {
                throw std::invalid_argument("Invalid arguments count for pos() function");
            } else {
                result.explicit_y_pos = std::stoi(get_list_el(args_list, 1));
            }

        } else if (fn_name_s == "move") {
            auto fn_args_s = style_ptr->substr(args_start + 1, args_end - args_start - 1);
            auto args_list = strutils::split(fn_args_s, ',');
            if (args_list.size() < 4 || args_list.size() > 6) {
                throw std::invalid_argument("Invalid arguments count for move() function");
            } else {
                auto y_begin = std::stoi(get_list_el(args_list, 1));
                auto y_end = std::stoi(get_list_el(args_list, 3));
                result.explicit_y_pos = std::round(std::abs(y_begin - y_end) / 2.0);
            }
        }
    }

    return result;
}

std::string field::parse_plain_text(std::string value)
{
    strutils::replace_all(value, "\\N", "\n");
    return value;
}

field::LineValuesParser::LineValuesParser()
{
    std::memset(this->parsers, 0, sizeof(size_t) * (UNKNOWN + 1));
    std::memset(this->result, 0, sizeof(size_t) * (UNKNOWN + 1));
    std::memset(this->dealloc, 0, sizeof(size_t) * (UNKNOWN + 1));
}

void field::LineValuesParser::parse(std::list<field::FieldType> &format, std::string &input_value)
{
    size_t str_pos = 0;
    for (auto field : format) {
        if (field == TEXT) {
            auto curr_value = text_col_value(input_value, str_pos);
            FIELD_PARSE_IF_NEEDED(field, curr_value, this->result);
            break; // Text is always the last field!
        } else {
            auto curr_value = col_value(input_value, str_pos);
            FIELD_PARSE_IF_NEEDED(field, curr_value, this->result);
        }
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

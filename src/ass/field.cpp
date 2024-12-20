#include <list>
#include <cstring>
#include <stdexcept>
#include <string>
#include "field.hpp"

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
    // TODO: Implement
    return {0, -1};
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

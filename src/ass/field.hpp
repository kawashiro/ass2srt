#ifndef ASS2SRT_ASS_FIELD_H
#define ASS2SRT_ASS_FIELD_H

#include "../strutils.hpp"
#include <array>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

// Text align calculation
#define ALIGN_LEFT 1
#define ALIGN_CENTER 2
#define ALIGN_RIGHT 3

#define ALIGN_BOTTOM 0
#define ALIGN_MIDDLE 3
#define ALIGN_TOP 6

#define ALIGN(Y, X) ((Y) + (X))

#define ALIGN_IS_BOTTOM(VAL) ((VAL) >= ALIGN(ALIGN_BOTTOM, ALIGN_LEFT) && (VAL) <= ALIGN(ALIGN_BOTTOM, ALIGN_RIGHT))
#define ALIGN_IS_MIDDLE(VAL) ((VAL) >= ALIGN(ALIGN_MIDDLE, ALIGN_LEFT) && (VAL) <= ALIGN(ALIGN_MIDDLE, ALIGN_RIGHT))
#define ALIGN_IS_TOP(VAL) ((VAL) >= ALIGN(ALIGN_TOP, ALIGN_LEFT) && (VAL) <= ALIGN(ALIGN_TOP, ALIGN_RIGHT))

#define ALIGN_DEFAULT ALIGN(ALIGN_MIDDLE, ALIGN_CENTER)

#define ALIGN_VALID(VAL) (ALIGN_IS_BOTTOM(VAL) || ALIGN_IS_MIDDLE(VAL) || ALIGN_IS_TOP(VAL))

namespace ass2srt::ass::field {
/**
 * Possible fields in the sections
 */
enum FieldType : std::uint8_t {
    NAME = 0,
    STYLE = 1,
    ALIGNMENT = 2,
    MARGIN_V = 3,
    START = 4,
    END = 5,
    TEXT = 6,
    UNKNOWN = 15
};

/**
 * Field info
 */
struct field_t {
    /**
     * Definition in the file as a string
     */
    const char* definition;

    /**
     * Unique field id
     */
    FieldType id;
};

/**
 * Parsed style values
 */
struct styles_spec_t {
    uint8_t alignment;
    int margin_v;
    float explicit_y_pos;
    bool is_drawing;
    explicit styles_spec_t();
    styles_spec_t(uint8_t, int, float, bool);
};

/**
 * Available fields
 */
const std::array<field_t, 7> fields_list = { {
    { .definition = "Name", .id = NAME },
    { .definition = "Style", .id = STYLE },
    { .definition = "Alignment", .id = ALIGNMENT },
    { .definition = "MarginV", .id = MARGIN_V },
    { .definition = "Start", .id = START },
    { .definition = "End", .id = END },
    { .definition = "Text", .id = TEXT },
} };

/**
 * Parse the field definition
 */
auto parse_type(const std::string&) -> FieldType;

/**
 * Parse ASS time string
 */
auto parse_time_millis(const std::string&) -> long;

/**
 * Parse ASS text inline styles
 */
auto parse_inline_style(const std::string&) -> styles_spec_t;

/**
 * Parse ergular text value from dialogue line
 */
auto parse_plain_text(const std::string&) -> std::string;

/**
 * Line parser
 */
class LineValuesParser {
public:
    /**
     * To add internal data allocation
     */
    explicit LineValuesParser();

    LineValuesParser(const LineValuesParser&) = delete;
    LineValuesParser(LineValuesParser&&) = delete;
    auto operator=(const LineValuesParser&) -> LineValuesParser& = delete;
    auto operator=(LineValuesParser&&) -> LineValuesParser& = delete;

    /**
     * Add parsing function for a field
     */
    template <typename T>
    void on(const FieldType field_type, T* (*parser_fn)(std::string&))
    {
        this->parsers[field_type] = reinterpret_cast<void* (*)(std::string&)>(parser_fn);
        this->dealloc[field_type] = [](void* ptr) { delete (T*)ptr; };
    }

    /**
     * Parse the line
     */
    void parse(const std::vector<field::FieldType>&, const std::string&);

    /**
     * Ge the parsed value
     */
    template <typename T>
    auto get(const FieldType field) const -> T
    {
        if (this->result[field] == nullptr) {
            auto field_name = strutils::format("0x%x", field);
            for (auto fld : fields_list) {
                if (fld.id == field) {
                    field_name = std::string(fld.definition);
                    break;
                }
            }
            throw std::invalid_argument(strutils::format("Field %s is not parsed", field_name.c_str()));
        }
        return *(T*)(this->result[field]);
    }

    /**
     * To free up internal results holder
     */
    ~LineValuesParser();

private:
    std::array<void*, UNKNOWN + 1> result;
    std::array<void* (*)(std::string&), UNKNOWN + 1> parsers;
    std::array<void (*)(void*), UNKNOWN + 1> dealloc;
};
}

#endif

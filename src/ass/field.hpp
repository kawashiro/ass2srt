#ifndef _ASS2SRT_ASS_FIELD_H
#define _ASS2SRT_ASS_FIELD_H

#include <list>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include "../strutils.hpp"

// Text align calculation
#define ALIGN_LEFT 1
#define ALIGN_CENTER 2
#define ALIGN_RIGHT 3

#define ALIGN_BOTTOM 0
#define ALIGN_MIDDLE 8
#define ALIGN_TOP 4

#define ALIGN(Y, X) ((Y) + (X))
#define ALIGN_VALID(VAL) ((VAL) >= ALIGN(ALIGN_BOTTOM, ALIGN_LEFT) && (VAL) <= ALIGN(ALIGN_MIDDLE, ALIGN_RIGHT))

#define ALIGN_IS_BOTTOM(VAL) ((VAL) < ALIGN_TOP)
#define ALIGN_IS_MIDDLE(VAL) ((VAL) > ALIGN_MIDDLE)
#define ALIGN_IS_TOP(VAL) ((VAL) > ALIGN_TOP && (VAL) < ALIGN_MIDDLE)

#define ALIGN_DEFAULT ALIGN(ALIGN_BOTTOM, ALIGN_CENTER)

namespace ass2srt::ass::field {
    /**
     * Possible fields in the sections
     */
    enum FieldType
    {
        NAME = 0,
        STYLE,
        ALIGNMENT,
        MARGIN_V,
        START,
        END,
        TEXT,
        UNKNOWN = 15
    };

    /**
     * Field info
     */
    struct field_t
    {
        /**
         * Definition in the file as a string
         */
        const char *definition;

        /**
         * Unique field id
         */
        const FieldType id;
    };

    /**
     * Parsed style values
     */
    struct styles_spec_t {
        uint8_t alignment;
        int margin_v;
        int explicit_y_pos;
        explicit styles_spec_t();
        styles_spec_t(uint8_t alignment, int margin_v, int explicit_v_pos);
        styles_spec_t(const styles_spec_t &);
        styles_spec_t& operator =(const styles_spec_t &);
    };

    /**
     * Available fields
     */
    const field_t fields_list[] = {
        { "Name", NAME },
        { "Style", STYLE },
        { "Alignment", ALIGNMENT },
        { "MarginV", MARGIN_V },
        { "Start", START },
        { "End", END },
        { "Text", TEXT },
    };

    /**
     * Parse the field definition
     */
    FieldType parse_type(std::string &);

    /**
     * Parse ASS time string
     */
    long parse_time_millis(std::string &);

    /**
     * Parse ASS text inline styles
     */
    styles_spec_t parse_inline_style(std::string &);

    /**
     * Parse ergular text value from dialogue line
     */
    std::string parse_plain_text(std::string);

    /**
     * Line parser
     */
    class LineValuesParser {
        public:
        /**
         * To add internal data allocation
         */
        explicit LineValuesParser();
        
        LineValuesParser(const LineValuesParser &) = delete;
        LineValuesParser &operator=(const LineValuesParser &) = delete;

        /**
         * Add parsing function for a field
         */
        template<typename T>
        void on(FieldType field_type, T *(*parser_fn)(std::string &))
        {
            this->parsers[field_type] = reinterpret_cast<void *(*)(std::string &)>(parser_fn);
            this->dealloc[field_type] = [](void *ptr) { delete (T*)ptr; };
        }

        /**
         * Parse the line
         */
        void parse(std::list<field::FieldType> &, std::string &);

        /**
         * Ge the parsed value
         */
        template<typename T>
        T get(FieldType field)
        {
            if (this->result[field] == nullptr) {
                throw std::invalid_argument(strutils::format("Field 0x%x is not parsed", field));
            }
            return *(T*)(this->result[field]);
        }

        /**
         * To free up internal results holder
         */
        ~LineValuesParser();

        private:
        void *result[UNKNOWN + 1];
        void *(*parsers[UNKNOWN + 1])(std::string &);
        void (*dealloc[UNKNOWN + 1])(void *);
    };
}

#endif

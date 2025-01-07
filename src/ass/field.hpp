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
        bool is_drawing;
        explicit styles_spec_t();
        styles_spec_t(uint8_t, int, int, bool);
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
    FieldType parse_type(const std::string &);

    /**
     * Parse ASS time string
     */
    long parse_time_millis(const std::string &);

    /**
     * Parse ASS text inline styles
     */
    styles_spec_t parse_inline_style(const std::string &);

    /**
     * Parse ergular text value from dialogue line
     */
    std::string parse_plain_text(const std::string);

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
        void on(const FieldType field_type, T *(*parser_fn)(std::string &))
        {
            this->parsers[field_type] = reinterpret_cast<void *(*)(std::string &)>(parser_fn);
            this->dealloc[field_type] = [](void *ptr) { delete (T*)ptr; };
        }

        /**
         * Parse the line
         */
        void parse(const std::list<field::FieldType> &, const std::string &);

        /**
         * Ge the parsed value
         */
        template<typename T>
        T get(const FieldType field) const
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
        void *result[UNKNOWN + 1];
        void *(*parsers[UNKNOWN + 1])(std::string &);
        void (*dealloc[UNKNOWN + 1])(void *);
    };
}

#endif

#include <list>
#include <set>
#include <stdexcept>
#include <string>
#include "field.hpp"
#include "line.hpp"
#include "../strutils.hpp"

using namespace ass2srt::ass;

line::LineType line::parse_type(const std::string &value)
{
    size_t separator_pos = value.find(':', 0);
    if (separator_pos == std::string::npos) {
        throw std::invalid_argument("Can not get the line type");
    }

    auto line_type_str = value.substr(0, separator_pos);
    for (auto line_def : line_type_list) {
        if (line_type_str == line_def.definition) {
            return line_def.id;
        }
    }

    return UNKNOWN;
}

std::string line::get_line_value(const std::string &input)
{
    size_t separator_pos = input.find(':', 0);
    auto line = input.substr(separator_pos + 1, input.length() - separator_pos - 1);
    strutils::trim(line);

    return line;
}

std::list<field::FieldType> line::parse_format_declaration(const std::string &input, const std::set<std::string> required_fields)
{
    auto str_parts = strutils::split(input, ',');
    std::set<std::string> required_fields_tmp = required_fields;

    std::list<field::FieldType> fields;
    std::set<std::string>::iterator t_pos;
    for (auto part : str_parts) {
        if ((t_pos = required_fields_tmp.find(part)) != required_fields_tmp.end()) {
            required_fields_tmp.erase(t_pos);
        }
        auto field_v = field::parse_type(part);
        fields.push_back(field_v);
    }

    if (!required_fields_tmp.empty()) {
        throw std::invalid_argument("Not all required fields found in the format definition");
    }

    return fields;
}

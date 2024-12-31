#include <stdexcept>
#include <string>
#include "section.hpp"

using namespace ass2srt::ass;

section::Section section::parse(const std::string &value)
{
    if (value.length() < 3 || value[0] != '[' || value[value.length() - 1] != ']') {
        throw std::invalid_argument("Invalid section declaration");
    }

    for (auto section : sections_list) {
        auto definition = std::string("[") + section.definition + "]";
        if (value == definition) {
            return section.id;
        }
    }

    return UNKNOWN;
}

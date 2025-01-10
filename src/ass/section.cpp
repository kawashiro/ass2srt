#include "section.hpp"
#include <stdexcept>
#include <string>

using namespace ass2srt::ass;

auto section::parse(const std::string& value) -> section::Section
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

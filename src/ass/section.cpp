#include <string>
#include "section.hpp"

using namespace ass2srt::ass;

section::Section section::parse(std::string &value)
{
    for (auto section : sections_list) {
        auto definition = std::string("[") + section.definition + "]";
        if (value == definition) {
            return section.id;
        }
    }

    return UNKNOWN;
}

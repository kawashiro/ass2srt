#ifndef ASS2SRT_STRUTILS_H
#define ASS2SRT_STRUTILS_H

#include "subline.hpp"
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace ass2srt::strutils {
/**
 * Trim from start (in place)
 */
void ltrim(std::string&);

/**
 * Trim from end (in place)
 */
void rtrim(std::string&);

/**
 * Trim from both sides (in place)
 */
void trim(std::string&);

/**
 * Deduplicate EOL char
 */
auto dedup_eol(const std::string&) -> std::string;

/**
 * Replace all the occurences of the string
 */
void replace_all(std::string&, const std::string&, const std::string&);

/**
 * Split string into parts
 */
auto split(std::string, char) -> std::vector<std::string>;

/**
 * Convert subtitles to string
 */
auto subtitles_to_string(const subtitles_t&) -> std::string;

/**
 * Safely format the string
 */
template <typename... T>
auto format(const std::string& format, T... args) -> std::string
{
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;
    if (size_s <= 0) {
        throw std::runtime_error("std::snprintf(nullptr, ...) error");
    }
    auto size = static_cast<size_t>(size_s);

    std::unique_ptr<char[]> buf(new char[size]); // NOLINT
    size_s = std::snprintf(buf.get(), size, format.c_str(), args...);
    if (size_s <= 0) {
        throw std::runtime_error("std::snprintf(buf, ...) error");
    }

    return { buf.get(), buf.get() + size - 1 };
}
}

#endif

#ifndef _ASS2SRT_STRUTILS_H
#define _ASS2SRT_STRUTILS_H

#include <exception>
#include <list>
#include <memory>
#include <string>
#include "subline.hpp"

namespace ass2srt::strutils {
    /**
     * Trim from start (in place)
     */
    void ltrim(std::string &);

    /**
     * Trim from end (in place)
     */
    void rtrim(std::string &);

    /**
     * Trim from both sides (in place)
     */
    void trim(std::string &);

    /**
     * Replace all the occurences of the string
     */
    void replace_all(std::string &, const std::string &, const std::string &);

    /**
     * Split string into parts
     */
    std::list<std::string> split(std::string, const char);

    /**
     * Convert subtitles to string
     */
    std::string subtitles_to_string(const subtitles_t &);

    /**
     * Safely format the string
     */
    template<typename... T>
    std::string format(const std::string &format, T... args)
    {
        int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;
        if (size_s <= 0) {
            throw std::runtime_error("Error during formatting the string");
        }
        auto size = static_cast<size_t>(size_s);

        std::unique_ptr<char[]> buf(new char[size]);
        std::snprintf(buf.get(), size, format.c_str(), args...);

        return std::string(buf.get(), buf.get() + size - 1);
    }
}

#endif

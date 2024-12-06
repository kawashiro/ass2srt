#ifndef _ASS2SRT_STRUTILS_H
#define _ASS2SRT_STRUTILS_H

#include <string>

namespace ass2srt { namespace strutils {
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
} }

#endif

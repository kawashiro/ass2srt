#ifndef ASS2SRT_IO_H
#define ASS2SRT_IO_H

#include <iostream>
#include <memory>
#include <string>

namespace ass2srt::io {
/**
 * Open input file
 */
auto open_istream(const std::string&) -> std::shared_ptr<std::istream>;

/**
 * Open output file
 */
auto open_ostream(const std::string&) -> std::shared_ptr<std::ostream>;
}

#endif

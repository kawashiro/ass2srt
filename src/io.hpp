#ifndef ASS2SRT_IO_H
#define ASS2SRT_IO_H

#include <fstream>
#include <string>

namespace ass2srt::io {
/**
 * Open input file
 */
auto open_ifstream(const std::string&) -> std::ifstream;

/**
 * Open output file
 */
auto open_ofstream(const std::string&) -> std::ofstream;
}

#endif

#ifndef ASS2SRT_IO_H
#define ASS2SRT_IO_H

#include <filesystem>
#include <iostream>
#include <memory>

namespace ass2srt::io {
/**
 * Open input file
 */
auto open_istream(const std::filesystem::path&) -> std::shared_ptr<std::istream>;

/**
 * Open output file
 */
auto open_ostream(const std::filesystem::path&) -> std::shared_ptr<std::ostream>;
}

#endif

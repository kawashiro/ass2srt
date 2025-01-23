#include "io.hpp"
#include "strutils.hpp"
#include <cerrno>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace ass2srt;

#define RAISE_ON_FAIL(S, N)                                                                                   \
    do {                                                                                                      \
        if ((S).fail()) {                                                                                     \
            auto err_msg = strutils::format("Failed to open file %s: %s", (N).c_str(), std::strerror(errno)); \
            throw std::runtime_error(err_msg);                                                                \
        }                                                                                                     \
    } while (0)

auto io::open_ifstream(const std::string& file_path) -> std::ifstream
{
    std::ifstream input_file(file_path, std::ios_base::in);
    RAISE_ON_FAIL(input_file, file_path);
    return input_file;
}

auto io::open_ofstream(const std::string& file_path) -> std::ofstream
{
    std::ofstream output_file(file_path, std::ios_base::out | std::ios_base::binary);
    RAISE_ON_FAIL(output_file, file_path);
    return output_file;
}

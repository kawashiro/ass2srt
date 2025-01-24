#include "io.hpp"
#include "strutils.hpp"
#include <cerrno>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

using namespace ass2srt;

#define RAISE_ON_FAIL(S, N)                                                                                   \
    do {                                                                                                      \
        if ((S)->fail()) {                                                                                    \
            auto err_msg = strutils::format("Failed to open file %s: %s", (N).c_str(), std::strerror(errno)); \
            throw std::runtime_error(err_msg);                                                                \
        }                                                                                                     \
    } while (0)

auto io::open_istream(const std::string& file_path) -> std::shared_ptr<std::istream>
{
    if (file_path.empty()) {
        return { &std::cin, [](std::istream*) { } };
    }

    auto input_file = std::make_shared<std::ifstream>(file_path, std::ios_base::in);
    RAISE_ON_FAIL(input_file, file_path);
    return input_file;
}

auto io::open_ostream(const std::string& file_path) -> std::shared_ptr<std::ostream>
{
    if (file_path.empty()) {
        return { &std::cout, [](std::ostream*) { } };
    }

    auto output_file = std::make_shared<std::ofstream>(file_path, std::ios_base::out | std::ios_base::binary);
    RAISE_ON_FAIL(output_file, file_path);
    return output_file;
}

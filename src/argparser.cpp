#include "argparser.hpp"
#include <set>
#include <string>
#include <unistd.h> // NOLINT

using namespace ass2srt;

namespace {
auto split_set_arg(char* input) -> std::set<std::string>
{
    std::set<std::string> res {};

    char* current = nullptr;
    for (;;) {
        if (current == nullptr) {
            current = input;
        } else if (*input == ',') {
            res.insert(std::string(current, input - current));
            current = nullptr;
        } else if (*input == 0) {
            res.insert(std::string(current, input - current));
            break;
        }
        ++input;
    }

    return res;
}
}

auto argparser::args::valid() const -> bool
{
    return !this->has_extra_opts && !this->input_file.empty() && !this->output_file.empty();
}

auto argparser::parse_args(int argc, char** argv) -> argparser::args
{
    std::string input_file;
    std::string output_file;
    std::set<std::string> styles_scope {};
    std::set<std::string> excluded_styles {};
    bool exclude_signs = false;
    bool has_extra_opt = false;

    int opt = -1;
    while ((opt = getopt(argc, argv, "i:o:s:e:x")) != -1) { // NOLINT
        switch (opt) {
        case 'i':
            input_file = optarg; // NOLINT
            break;
        case 'o':
            output_file = optarg;
            break;
        case 's':
            styles_scope = split_set_arg(optarg);
            break;
        case 'e':
            excluded_styles = split_set_arg(optarg);
            break;
        case 'x':
            exclude_signs = true;
            break;
        default:
            has_extra_opt = true;
        }
    }

    return {
        .input_file = input_file,
        .output_file = output_file,
        .styles_scope = styles_scope,
        .excluded_styles = excluded_styles,
        .exclude_signs = exclude_signs,
        .has_extra_opts = has_extra_opt
    };
}

#include <set>
#include <string>
#include <unistd.h>
#include "argparser.hpp"

using namespace ass2srt;

static const std::set<std::string> split_set_arg(char *input)
{
    std::set<std::string> res {};
    
    char *current = nullptr;
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

bool argparser::args::is_valid() const
{
    return !this->has_extra_opts && !this->input_file.empty() && !this->output_file.empty();
}

const argparser::args argparser::parse_args(int argc, char **argv)
{
    std::string input_file = "";
    std::string output_file = "";
    std::set<std::string> styles_scope {};
    std::set<std::string> excluded_styles {};
    bool exclude_signs = false;
    bool has_extra_opt = false;

    char opt;
    while ((opt = getopt(argc, argv, "i:o:s:e:x")) != -1) {
        switch (opt) {
        case 'i':
            input_file = optarg;
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
        input_file,
        output_file,
        styles_scope,
        excluded_styles,
        exclude_signs,
        has_extra_opt
     };
}

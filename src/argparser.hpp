#ifndef _ASS2SRT_ARGPARSER_H
#define _ASS2SRT_ARGPARSER_H

#include <set>
#include <string>

namespace ass2srt::argparser {
    /**
     * Config passed by input args
     */
    struct args {
        /**
         * Input file
         */
        const std::string input_file;

        /**
         * Output file
         */
        const std::string output_file;

        /**
         * Limit the output only by this subset of styles
         */
        const std::set<std::string> styles_scope;

        /**
         * Excluded styles (will not be included in output file)
         */
        const std::set<std::string> excluded_styles;

        /**
         * Flag to try to exclude all signs-like styles, to be appended to excluded_styles
         */
        const bool exclude_signs;

        /**
         * If there are any extra options
         */
        const bool has_extra_opts;

        /**
         * Check the validness
         */
        bool is_valid() const;
    };

    /**
     * Parser arguments
     */
    const args parse_args(int argc, char **argv);
}

#endif

#include <stdio.h>
#include "argparser.hpp"
#include "config.h"
#include "somemodule.hpp"

using namespace ass2srt;

int main(int argc, char **argv)
{
    const argparser::runtime_config params = argparser::parse_args(argc, argv);
    if (!params.is_valid()) {
        printf("ass2srt converter v.%d.%d.%d\n", ASS2SRT_VERSION_MAJOR, ASS2SRT_VERSION_MINOR, ASS2SRT_VERSION_PATCH);
        printf("Usage: ass2srt -i input_file -o output_file [-s styles_to_display] [-e excluded_styles] [ -x ]\n");
        return 2;
    }

    int test = get42();
    printf("Test val is %d\n", test);
    return 0;
}

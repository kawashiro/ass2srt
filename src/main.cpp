#include <iostream>
#include "config.h"
#include "somemodule.hpp"

int main(int argc, char **argv)
{
    int test = ass2srt::get42();
    std::cout << "Hello, ass2srt v" << ASS2SRT_VERSION_MAJOR << "." << ASS2SRT_VERSION_MINOR << "." << ASS2SRT_VERSION_PATCH << std::endl;
    std::cout << test << std::endl;
    return 0;
}

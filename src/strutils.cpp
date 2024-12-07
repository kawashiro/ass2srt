#include <algorithm> 
#include <cctype>
#include <locale>
#include <string>
#include "strutils.hpp"

using namespace ass2srt;

void strutils::ltrim(std::string &string) {
    string.erase(
        string.begin(),
        std::find_if(
            string.begin(),
            string.end(),
            [](unsigned char ch) {
                return !std::isspace(ch);
            }
        )
    );
}

void strutils::rtrim(std::string &string) {
    string.erase(
        std::find_if(
            string.rbegin(), 
            string.rend(),
            [](unsigned char ch) {
                return !std::isspace(ch);
            }
        ).base(),
        string.end()
    );
}

void strutils::trim(std::string &string) {
    strutils::rtrim(string);
    strutils::ltrim(string);
}


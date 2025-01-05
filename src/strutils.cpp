#include <algorithm> 
#include <cctype>
#include <list>
#include <locale>
#include <string>
#include "strutils.hpp"

using namespace ass2srt;

void strutils::ltrim(std::string &string)
{
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

void strutils::rtrim(std::string &string)
{
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

void strutils::trim(std::string &string)
{
    strutils::rtrim(string);
    strutils::ltrim(string);
}

void strutils::replace_all(std::string &value, const std::string &search, const std::string &replacement)
{
    size_t pos = 0;
    while ((pos = value.find(search, pos)) != std::string::npos) {
        value.replace(pos, search.length(), replacement);
        pos += replacement.length() - search.length() + 1;
    }
}

std::list<std::string> strutils::split(std::string input, const char delimiter)
{
    std::list<std::string> parts;
    size_t pos = 0;
    std::string token;
    while ((pos = input.find(delimiter)) != std::string::npos) {
        token = input.substr(0, pos);
        strutils::trim(token);
        parts.push_back(token);
        input.erase(0, pos + 1);
    }
    strutils::trim(input);
    parts.push_back(input);

    return parts;
}
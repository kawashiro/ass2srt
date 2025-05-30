#include "strutils.hpp"
#include "subline.hpp"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <string>
#include <vector>

using namespace ass2srt;

void strutils::ltrim(std::string& string)
{
    string.erase(
        string.begin(),
        std::ranges::find_if(
            string,
            [](unsigned char symbol) {
                return !std::isspace(symbol);
            }));
}

void strutils::rtrim(std::string& string)
{
    string.erase(
        std::ranges::find_if(
            string.rbegin(),
            string.rend(),
            [](unsigned char symbol) {
                return !std::isspace(symbol);
            })
            .base(),
        string.end());
}

void strutils::trim(std::string& string)
{
    strutils::rtrim(string);
    strutils::ltrim(string);
}

void strutils::replace_all(std::string& value, const std::string& search, const std::string& replacement)
{
    size_t pos = 0;
    while ((pos = value.find(search, pos)) != std::string::npos) {
        value.replace(pos, search.length(), replacement);
        pos += replacement.length() - search.length() + 1;
    }
}

auto strutils::split(std::string input, const char delimiter) -> std::vector<std::string>
{
    std::vector<std::string> parts;
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

auto strutils::subtitles_to_string(const subtitles_t& subtitles) -> std::string
{
    const std::string indent("    ");
    const std::string indent2 = indent + indent;
    const std::string indent3 = indent2 + indent;
    const std::string indent4 = indent3 + indent;

    std::string output("{\n");
    for (const auto& subline : subtitles) {
        output += indent + "{\n";
        output += indent2 + std::to_string(subline.start_milis) + ",\n";
        output += indent2 + std::to_string(subline.end_milis) + ",\n";
        output += indent2 + "{\n";
        for (const auto& part : subline.parts) {
            output += indent3 + "{\n";
            output += indent4 + std::to_string((long double)part.v_pos) + ",\n";
            output += indent4 + std::to_string(part.x_order) + ",\n";
            output += indent4 + "\"" + part.text + "\",\n";
            output += indent3 + "},\n";
        }
        output += indent2 + "},\n";
        output += indent + "},\n";
    }
    output += "}";
    return output;
}

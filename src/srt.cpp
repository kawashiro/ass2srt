#include "srt.hpp"
#include "strutils.hpp"
#include "subline.hpp"
#include <cstdint>
#include <cstdlib>
#include <ostream>
#include <string>

using namespace ass2srt;

namespace {
const char* const srt_eol = "\r\n";

struct srt_time {
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
    uint8_t millis100;

    [[nodiscard]] auto str() const -> std::string
    {
        return strutils::format("%.2u:%.2u:%.2u,%.2u0", hours, minutes, seconds, millis100);
    }
};

auto parse_millis(const long millis) -> srt_time
{
    const int millis_mult = 10;
    const int seconds_mult = 1000;
    const int minutes_mult = 60 * 1000;
    const int hours_mult = 60 * 60 * 1000;

    auto hours_dv = std::ldiv(millis, hours_mult);
    auto minutes_dv = std::ldiv(hours_dv.rem, minutes_mult);
    auto seconds_dv = std::ldiv(minutes_dv.rem, seconds_mult);
    auto millis100 = seconds_dv.rem / millis_mult;

    return {
        .hours = static_cast<uint8_t>(hours_dv.quot),
        .minutes = static_cast<uint8_t>(minutes_dv.quot),
        .seconds = static_cast<uint8_t>(seconds_dv.quot),
        .millis100 = static_cast<uint8_t>(millis100),
    };
}

auto convert_eol(const std::string& text) -> std::string
{
    std::string text_copy = text;
    strutils::replace_all(text_copy, "\n", std::string(srt_eol));
    return text_copy;
}
}

void srt::write_srt_stream(const subtitles_t& subtitles, std::ostream& out)
{
    unsigned int line_no = 1;
    const size_t size = subtitles.size();
    for (const auto& line : subtitles) {
        out << line_no << srt_eol;
        out << parse_millis(line.start_milis).str() << " --> " << parse_millis(line.end_milis).str() << srt_eol;
        out << convert_eol(line.parts.front().text) << srt_eol;
        if (line_no < size) {
            out << srt_eol;
        }
        line_no++;
    }
}

#include <cstdint>
#include <cstdlib>
#include <ostream>
#include <string>
#include "srt.hpp"
#include "strutils.hpp"

using namespace ass2srt;

static const char *srt_eol = "\r\n";

struct srt_time
{
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
    uint8_t millis100;

    inline std::string str() const
    {
        return strutils::format("%.2u:%.2u:%.2u,%.2u0", hours, minutes, seconds, millis100);
    }
};

static const srt_time parse_millis(const long millis)
{
    auto hours_dv = std::ldiv(millis, 60 * 60 * 1000);
    auto minutes_dv = std::ldiv(hours_dv.rem, 60 * 1000);
    auto seconds_dv = std::ldiv(minutes_dv.rem, 1000);
    auto millis100 = seconds_dv.rem / 10;

    return {
        static_cast<uint8_t>(hours_dv.quot),
        static_cast<uint8_t>(minutes_dv.quot),
        static_cast<uint8_t>(seconds_dv.quot),
        static_cast<uint8_t>(millis100),
    };
}

static const std::string convert_eol(const std::string &text)
{
    std::string text_copy = text;
    strutils::replace_all(text_copy, "\n", std::string(srt_eol));
    return text_copy;
}

void srt::write_srt_stream(const subtitles_t &subtitles, std::ostream &out)
{
    unsigned int line_no = 1;
    size_t size = subtitles.size();
    for (auto line_it = subtitles.begin(); line_it != subtitles.end(); ++line_it) {
        out << line_no << srt_eol;
        out << parse_millis(line_it->start_milis).str() << " --> " << parse_millis(line_it->end_milis).str() << srt_eol;
        out << convert_eol(line_it->parts.front().text) << srt_eol;
        if (line_no < size) {
            out << srt_eol;
        }
        line_no++;
    }
}

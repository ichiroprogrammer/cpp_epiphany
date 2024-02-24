#pragma once
#include <string>

// @@@ sample begin 0:0

inline std::string line_to_str(uint32_t line)
{
    if (line < 10) {
        return ":  ";
    }
    else if (line < 100) {
        return ": ";
    }
    else if (line < 1000) {
        return ":";
    }
    else {
        assert(false);  // 1000行を超える単体テストファイルを認めない
        return "";
    }
}

inline std::string log_str_exp(char const* filename_cstr, uint32_t line, char const* str)
{
    auto const filename = std::string{filename_cstr};
    auto const len      = 32 > filename.size() ? 32 - filename.size() : 0;
    auto       ret      = std::string(len, ' ');

    ret += filename;
    ret += line_to_str(line);
    ret += std::to_string(line);
    ret += str;

    return ret;
}
// @@@ sample end

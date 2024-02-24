#pragma once

#include <sstream>

// @@@ sample begin 0:0

namespace Logging {
class Logger {
public:
    static Logger&       Inst();
    static Logger const& InstConst() { return Inst(); }

    std::string Get() const;  // ログデータの取得
    void        Clear();      // ログデータの消去

    template <typename... ARGS>  // ログの登録
    void Set(char const* filename, uint32_t line_no, ARGS const&... args)
    {
        oss_.width(32);
        oss_ << filename << ":";

        oss_.width(3);
        oss_ << line_no;

        set_inner(args...);
    }

    Logger(Logger const&)            = delete;
    Logger& operator=(Logger const&) = delete;

private:
    void set_inner() { oss_ << std::endl; }

    template <typename HEAD, typename... TAIL>
    void set_inner(HEAD const& head, TAIL const&... tails)
    {
        oss_ << ":" << head;
        set_inner(tails...);
    }

    Logger() {}
    std::ostringstream oss_{};
};
}  // namespace Logging
// @@@ sample end

// @@@ sample begin 0:1

#define LOGGER_P(...) Logging::Logger::Inst().Set(__FILE__, __LINE__)
#define LOGGER(...) Logging::Logger::Inst().Set(__FILE__, __LINE__, __VA_ARGS__)
// @@@ sample end

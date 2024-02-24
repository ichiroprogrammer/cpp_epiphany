#include <list>

#include "gtest_wrapper.h"

#include "app_ints.h"
#include "logger.h"
#include "logger_ut.h"

namespace {

TEST(Template, logger_built_in)
{
    Logging::Logger::Inst().Clear();

    auto        a = 1;
    int         a2[2]{2, 3};
    std::string b3[3]{"a", "b", "c"};

    LOGGER_P();                    // (1)
    LOGGER(5, "hehe", a, a2, b3);  // (2)
    auto line_num = __LINE__;

    auto s = Logging::Logger::InstConst().Get();

    auto exp = std::string{log_str_exp(__FILE__, line_num - 2, "\n")};         // (1)のログ
    exp += log_str_exp(__FILE__, line_num - 1, ":5:hehe:1:2, 3:a | b | c\n");  // (2)のログ
    ASSERT_EQ(exp, s);

    Logging::Logger::Inst().Clear();  // クリアの確認
    ASSERT_EQ("", Logging::Logger::InstConst().Get());
}

TEST(Template, logger_X)
{
    Logging::Logger::Inst().Clear();

    // @@@ sample begin 0:0

    using namespace Nstd;

    auto         x  = App::X{"name", 3};
    auto         lx = std::list<App::X>{{"lx3", 3}, {"lx4", 1}};
    App::X const x3[3]{{"x0", 0}, {"x1", 1}, {"x2", 2}};

    LOGGER(1, x, x3, lx);
    auto line_num = __LINE__;

    auto s = Logging::Logger::InstConst().Get();

    auto const exp
        = log_str_exp(__FILE__, line_num - 1, ":1:name/3:x0/0, x1/1, x2/2:lx3/3, lx4/1\n");
    ASSERT_EQ(exp, s);
    // @@@ sample end
}
}  // namespace

// clang-format off
#define INTS_UT_NAME logger_Ints
#include "logger_0_ints_ut.h"
// clang-format on

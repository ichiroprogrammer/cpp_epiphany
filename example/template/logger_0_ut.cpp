#include <cassert>

#include "gtest_wrapper.h"

#include "app_ints.h"
#include "logger_0.h"
#include "logger_ut.h"
#include "suppress_warning.h"

namespace {

TEST(Template, logger_0)
{
    Logging::Logger::Inst().Clear();

    // @@@ sample begin 0:0

    auto a = 1;
    auto b = std::string{"b"};

    LOGGER_P();               // (1)
    LOGGER(5, "hehe", a, b);  // (2)
    auto line_num = __LINE__;

    auto s = Logging::Logger::InstConst().Get();

    auto exp = std::string{log_str_exp(__FILE__, line_num - 2, "\n")};  // (1)のログ
    exp += log_str_exp(__FILE__, line_num - 1, ":5:hehe:1:b\n");        // (2)のログ
    ASSERT_EQ(exp, s);

    Logging::Logger::Inst().Clear();  // クリアの確認
    ASSERT_EQ("", Logging::Logger::InstConst().Get());
    // @@@ sample end
}
}  // namespace

namespace {
TEST(Template, logger_0_X)
{
    Logging::Logger::Inst().Clear();

    // @@@ sample begin 1:2

    auto x = App::X{"name", 3};

    LOGGER(1, x);
    auto line_num = __LINE__;

    auto s = Logging::Logger::InstConst().Get();

    auto exp = std::string{log_str_exp(__FILE__, line_num - 1, ":1:name/3\n")};
    ASSERT_EQ(exp, s);
    // @@@ sample end

    auto s1 = x.ToString();
    ASSERT_EQ("name/3", s1);

    auto oss = std::ostringstream{};
    oss << x;

    auto s2 = oss.str();
    ASSERT_EQ("name/3", s2);
}
}  // namespace

#define SHOW_COMPILE_ERROR 0
#if SHOW_COMPILE_ERROR == 1
// @@@ sample begin 2:0

namespace App2 {
class X {
public:
    explicit X(std::string str, int num) : str_{std::move(str)}, num_{num} {}
    std::string ToString() const { return str_ + "/" + std::to_string(num_); }
    // @@@ ignore begin
private:
    std::string const str_;
    int const         num_;
    // @@@ ignore end
};
}  // namespace App2

namespace App3 {  // App3をApp2にすればコンパイルできる
std::ostream& operator<<(std::ostream& os, App2::X const& x) { return os << x.ToString(); }
}  // namespace App3

namespace {

TEST(Template, logger_0_X_in_AppX)
{
    Logging::Logger::Inst().Clear();

    auto x = App2::X{"name", 3};

    using namespace App3;  // この記述は下記のエラーに効果がない

    LOGGER(1, x);  // ここがコンパイルエラーとなる
    auto line_num = __LINE__;

    auto s = Logging::Logger::InstConst().Get();

    auto exp = std::string{log_str_exp(__FILE__, line_num - 1, ":1:name/3\n")};
    ASSERT_EQ(exp, s);
}
}  // namespace
// @@@ sample end
#endif

// @@@ sample begin 3:0

namespace App {
std::ostream& operator<<(std::ostream& os, Ints_t const& ints)
{
    auto first = true;

    for (auto const i : ints) {
        if (!std::exchange(first, false)) {
            os << ", ";
        }
        os << i;
    }

    return os;
}
}  // namespace App
// @@@ sample end

namespace {

#define SHOW_COMPILE_ERROR2 0
#if SHOW_COMPILE_ERROR2 == 1
TEST(Template, operator_put_to)
{
    // @@@ sample begin 3:1

    auto ints = App::Ints_t{1, 2, 3};

    auto oss = std::ostringstream{};

    oss << ints;
    ASSERT_EQ("1, 2, 3", oss.str());
    // @@@ sample end
}
#endif
}  // namespace

#if SHOW_COMPILE_ERROR2 == 1
// clang-format off
#define INTS_UT_NAME logger_0_Ints
#include "logger_0_ints_ut.h"
// clang-format on
#endif

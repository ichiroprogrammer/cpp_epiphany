#include <cassert>

#include "gtest_wrapper.h"

#include "app_ints.h"
#include "logger_0.h"
#include "logger_ut.h"
#include "suppress_warning.h"

// @@@ sample begin 0:0

namespace App {  // App::Ints_tのoperator<<とToStringをApp内で定義
namespace {      // operator<<は外部から使わない
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
}  // namespace

// Ints_tオブジェクトをstd::stringに変換する
// この変換によりロガーに渡すことができる
std::string ToString(Ints_t const& inst)
{
    auto oss = std::ostringstream{};

    oss << inst;

    return oss.str();
}
}  // namespace App
// @@@ sample end

namespace {
TEST(Template, logger_0_Ints_no_put_to)
{
    Logging::Logger::Inst().Clear();

    // @@@ sample begin 0:1

    auto ints = App::Ints_t{1, 2, 3};

    // ToStringのテスト
    ASSERT_EQ("1, 2, 3", App::ToString(ints));

    // LOGGERのテスト
    LOGGER("Ints", App::ToString(ints));
    auto line_num = __LINE__;

    auto s = Logging::Logger::InstConst().Get();

    auto exp = std::string{log_str_exp(__FILE__, line_num - 1, ":Ints:1, 2, 3\n")};
    ASSERT_EQ(exp, s);
    // @@@ sample end
}
}  // namespace

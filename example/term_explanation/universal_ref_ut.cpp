#include <type_traits>

#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace UniversalRef {
// @@@ sample begin 0:0

template <typename T>
void f(T&& t) noexcept  // tはユニバーサルリファレンス
{
    // @@@ ignore begin
    IGNORE_UNUSED_VAR(t);
    // @@@ ignore end
}

template <typename T>
void g(std::vector<T>&& t) noexcept  // tはrvalueリファレンス
{
    // @@@ ignore begin
    IGNORE_UNUSED_VAR(t);
    // @@@ ignore end
}
// @@@ sample end

void call_template_func()
{
    // @@@ sample begin 0:1

    auto       vec  = std::vector<std::string>{"lvalue"};   // vecはlvalue
    auto const cvec = std::vector<std::string>{"clvalue"};  // cvecはconstなlvalue

    f(vec);                                 // 引数はlvalue
    f(cvec);                                // 引数はconstなlvalue
    f(std::vector<std::string>{"rvalue"});  // 引数はrvalue

    // g(vec);  // 引数がlvalueなのでコンパイルエラー
    // g(cvec); // 引数がconst lvalueなのでコンパイルエラー
    g(std::vector<std::string>{"rvalue"});  // 引数はrvalue
    // @@@ sample end
}

namespace {
TEST(UniversalRef, universal_ref)
{
    // @@@ sample begin 0:2

    // sはユニバーサルリファレンス
    auto value_type = [](auto&& s) noexcept {
        if (std::is_same_v<std::string&, decltype(s)>) {
            return 0;
        }
        if (std::is_same_v<std::string const&, decltype(s)>) {
            return 1;
        }
        if (std::is_same_v<std::string&&, decltype(s)>) {
            return 2;
        }
        return 3;
    };

    auto       str  = std::string{"lvalue"};
    auto const cstr = std::string{"const lvalue"};

    ASSERT_EQ(0, value_type(str));
    ASSERT_EQ(1, value_type(cstr));
    ASSERT_EQ(2, value_type(std::string{"rvalue"}));
    // @@@ sample end
}
}  // namespace
}  // namespace UniversalRef

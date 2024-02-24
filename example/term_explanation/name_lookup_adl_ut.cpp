#include "gtest_wrapper.h"

#include "suppress_warning.h"

// @@@ sample begin 0:0
namespace NS_ADL {
struct A {
    int i;
};

std::string ToString(A const& a) { return std::string{"A:"} + std::to_string(a.i); }
}  // namespace NS_ADL
// @@@ sample end

namespace {
TEST(Lookup, adl)
{
    // @@@ sample begin 0:1

    auto a = NS_ADL::A{0};

    ASSERT_EQ("A:0", ToString(a));  // ADLの効果により、ToStringはNS_ADLを指定しなくても見つかる
    // @@@ sample end
}

void f()
{
    // @@@ sample begin 0:2

    // 下記operator <<は、std::operator<<(ostream&, string const&)であり、
    // namespace stdで定義されている。

    // ADLがあるため、operator <<は名前空間修飾無しで呼び出せる。
    std::cout << std::string{__func__};

    // ADLが無いと下記のような呼び出しになる。
    std::operator<<(std::cout, std::string{__func__});
    // @@@ sample end
}

TEST(Lookup, adl_std) { f(); }
}  // namespace

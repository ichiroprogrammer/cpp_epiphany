#include <vector>

#include "gtest_wrapper.h"

#include "nstd_put_to.h"
#include "nstd_type2str.h"

namespace Type2Str {
TEST(Template, type2str)
{
    // @@@ sample begin 0:0

    ASSERT_EQ("int const", Nstd::Type2Str<int const>());
    ASSERT_EQ("std::string", Nstd::Type2Str<std::string>());
    ASSERT_EQ("std::vector<int, std::allocator<int> >", Nstd::Type2Str<std::vector<int>>());

    extern void f(int);
    ASSERT_EQ("void (int)", Nstd::Type2Str<decltype(f)>());  // 関数の型

    auto lamda = []() noexcept {};
    ASSERT_NE("", Nstd::Type2Str<decltype(lamda)>());  // XXX::{lambda()#1}な感じになる

    ASSERT_EQ("std::ostream& (std::ostream&, int const (&) [3])",
              Nstd::Type2Str<decltype(Nstd::operator<< <int[3]>)>());

    // std::declvalはrvalueリファレンスを返す
    ASSERT_EQ("int (&&) [3]", Nstd::Type2Str<decltype(std::declval<int[3]>())>());

    int i3[3];
    ASSERT_EQ("int [3]", Nstd::Type2Str<decltype(i3)>());
    ASSERT_EQ("int (&) [3]", Nstd::Type2Str<decltype((i3))>());  // (i3)はlvalueリファレンス

    auto& r = i3;
    ASSERT_EQ("int (&) [3]", Nstd::Type2Str<decltype(r)>());
    // @@@ sample end
}
}  // namespace Type2Str

#include <iostream>

#include "gtest_wrapper.h"

namespace {

// 本来Baseのデストラクタはvirtualでなければならないが、オブジェクトレイアウトを単純にするために
// 非virtualにする。

// @@@ sample begin 0:0

class Base {
public:
    explicit Base(char const* name) noexcept : name0_{name} {}
    char const* Name0() const noexcept { return name0_; }

    // @@@ ignore begin
    Base& operator=(Base const& rhs) = default;
    Base(Base const&)                = default;
    // @@@ ignore end
private:
    char const* name0_;
};

class Derived final : public Base {
public:
    Derived(char const* name0, char const* name1) noexcept : Base{name0}, name1_{name1} {}
    char const* Name1() const noexcept { return name1_; }

    // @@@ ignore begin
    Derived& operator=(Derived const& rhs) = default;
    Derived(Derived const&)                = default;
    // @@@ ignore end
private:
    char const* name1_;
};

TEST(Slicing, reference)
{
    auto const d0     = Derived{"d0", "d0"};
    auto const d1     = Derived{"d1", "d1"};
    auto       d2     = Derived{"d2", "d2"};
    Base&      d2_ref = d2;

    ASSERT_STREQ("d2", d2.Name0());  // OK
    ASSERT_STREQ("d2", d2.Name1());  // OK

    d2 = d0;
    ASSERT_STREQ("d0", d2.Name0());  // OK
    ASSERT_STREQ("d0", d2.Name1());  // OK

    d2_ref = d1;                     // d2_refはBase&型で、d2へのリファレンス
    ASSERT_STREQ("d1", d2.Name0());  // OK
#if 0
    ASSERT_STREQ("d1", d2.Name1());  // 本来ならこうなってほしいが、
#else
    ASSERT_STREQ("d0", d2.Name1());  // スライシングの影響でDerived::name1_はコピーされない
#endif
}
// @@@ sample end
// @@@ sample begin 1:0

TEST(Slicing, array)
{
    Derived d_array[]{{"0", "1"}, {"2", "3"}};
    Base*   b_ptr = d_array;  // この代入までは問題ないが、b_ptr[1]でのアクセスで問題が起こる

    ASSERT_STREQ("0", d_array[0].Name0());  // OK
    ASSERT_STREQ("0", b_ptr[0].Name0());    // OK

    ASSERT_STREQ("2", d_array[1].Name0());  // OK
#if 0  // スライシングに類似した問題で、以下のテストは失敗する。
    ASSERT_STREQ("2", b_ptr[1].Name0());  // NG
#else  // こうすればテストは通るが、、、
    ASSERT_STREQ("1", b_ptr[1].Name0());  // NG
#endif
}
// @@@ sample end
}  // namespace

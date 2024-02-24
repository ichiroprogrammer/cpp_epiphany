#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace NonVirtualDiamond {
// @@@ sample begin 0:0

class Base {
public:
    int32_t get() const noexcept { return x_; }
    void    set(int32_t x) noexcept { x_ = x; }

private:
    int32_t x_ = 0;
};

class Derived_0 : public Base {};

class Derived_1 : public Base {};

class DerivedDerived : public Derived_0, public Derived_1 {};
// @@@ sample end

TEST(DiamondInheritance, non_virtual_diamond)
{
    // @@@ sample begin 0:1

    auto dd = DerivedDerived{};

    Base& b0 = static_cast<Derived_0&>(dd);  // Derived_0::Baseのリファレンス
    Base& b1 = static_cast<Derived_1&>(dd);  // Derived_1::Baseのリファレンス

    ASSERT_NE(&b0, &b1);  // ddの中には、Baseインスタンスが2つできる
    // @@@ sample end
#if 0
    // @@@ sample begin 0:2

    Base& b = dd;  // Derived_0::Base or Derived_1::Base ?

    dd.get();  // Derived_0::get or  Derived_1::get ?

    // 下記のようなエラーが発生する
    //  diamond_inheritance_ut.cpp:37:15: error: ‘Base’ is an ambiguous base of ‘DerivedDerived’
    //     37 |     Base& b = dd;  // Derived_0::Base or Derived_1::Base ?
    //        |               ^~
    //  diamond_inheritance_ut.cpp:39:8: error: request for member ‘get’ is ambiguous
    //     39 |     dd.get();  // Derived_0::get or  Derived_1::get ?
    //        |        ^~~
    // @@@ sample end
    IGNORE_UNUSED_VAR(b);
#endif

    // @@@ sample begin 0:3

    ASSERT_EQ(0, dd.Derived_0::get());  // クラス名による名前修飾
    ASSERT_EQ(0, dd.Derived_1::get());

    dd.Derived_0::set(1);
    ASSERT_EQ(1, dd.Derived_0::get());  // Derived_0::Base::x_は1に変更
    ASSERT_EQ(0, dd.Derived_1::get());  // Derived_1::Base::x_は0のまま

    dd.Derived_1::set(2);
    ASSERT_EQ(1, dd.Derived_0::get());  // Derived_0::Base::x_は1のまま
    ASSERT_EQ(2, dd.Derived_1::get());  // Derived_1::Base::x_は2に変更
    // @@@ sample end
}
}  // namespace NonVirtualDiamond

namespace VirtualDiamond {
// @@@ sample begin 1:0

class Base {
public:
    int32_t get() const noexcept { return x_; }
    void    set(int32_t x) noexcept { x_ = x; }

private:
    int32_t x_ = 0;
};

class Derived_0 : public virtual Base {};  // 仮想継承

class Derived_1 : public virtual Base {};  // 仮想継承

class DerivedDerived : public Derived_0, public Derived_1 {};
// @@@ sample end

TEST(DiamondInheritance, virtual_diamond)
{
    // @@@ sample begin 1:1

    auto dd = DerivedDerived{};

    Base& b0 = static_cast<Derived_0&>(dd);  // Derived_0::Baseのリファレンス
    Base& b1 = static_cast<Derived_1&>(dd);  // Derived_1::Baseのリファレンス

    ASSERT_EQ(&b0, &b1);  // ddの中には、Baseインスタンスが1つできる
    // @@@ sample end
    // @@@ sample begin 1:2

    Base& b = dd;  // Baseインスタンスは1つであるため、コンパイルできる

    dd.get();  // Baseインスタンスは1つであるため、コンパイルできる

    dd.Derived_0::set(1);               // クラス名による修飾
    ASSERT_EQ(1, dd.Derived_1::get());  // Derived_1::BaseとDerived_1::Baseは同一であるため

    dd.set(2);
    ASSERT_EQ(2, dd.get());
    // @@@ sample end

    IGNORE_UNUSED_VAR(b);
}
}  // namespace VirtualDiamond

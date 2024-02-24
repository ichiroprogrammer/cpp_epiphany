#include <cassert>

#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace VirtualInheritance {

// @@@ sample begin 0:0

class Base {
public:
    explicit Base(int32_t x = 0) noexcept : x_{x} {}
    int32_t get() const noexcept { return x_; }

private:
    int32_t x_;
};

class DerivedVirtual : public virtual Base {  // 仮想継承
public:
    explicit DerivedVirtual(int32_t x) noexcept : Base{x} {}
};
// @@@ sample end
// @@@ sample begin 0:1

class DerivedDerivedVirtual : public DerivedVirtual {  // 仮想継承を通常の継承
public:
    explicit DerivedDerivedVirtual(int32_t x) noexcept : DerivedVirtual{x} {}
};

class DerivedNormal : public Base {  // 通常の継承
public:
    explicit DerivedNormal(int32_t x) noexcept : Base{x} {}
};

class DerivedDerivedNormal : public DerivedNormal {  // 通常の継承を通常の継承
public:
    explicit DerivedDerivedNormal(int32_t x) noexcept : DerivedNormal{x} {}
};
// @@@ sample end

TEST(VirtualInheritance, virtual_inherit_0)
{
    // @@@ sample begin 0:2

    auto dv = DerivedVirtual{1};  // 仮想継承クラス
    auto dn = DerivedNormal{1};   // 通常の継承クラス

    ASSERT_EQ(1, dv.get());  // これは非仮想継承と同じ動作
    ASSERT_EQ(1, dn.get());

    auto ddv = DerivedDerivedVirtual{1};  // 仮想継承クラスを継承したクラス
    auto ddn = DerivedDerivedNormal{1};   // 通常の継承クラスを継承したクラス

    ASSERT_EQ(0, ddv.get());  // Baseのデフォルトコンストラクタが呼ばれる
    ASSERT_EQ(1, ddn.get());
    // @@@ sample end
}

// @@@ sample begin 0:3

class DerivedDerivedVirtualFixed : public DerivedVirtual {  // DerivedDerivedNormalと同じように動作
public:
    explicit DerivedDerivedVirtualFixed(int32_t x) noexcept : Base{x}, DerivedVirtual{x} {}
    //                     基底クラスのコンストラクタ呼び出し ^^^^^^^
};
// @@@ sample end

TEST(VirtualInheritance, virtual_inherit_1)
{
    // @@@ sample begin 0:4

    DerivedDerivedVirtual ddv{1};  // 仮想継承クラスを継承したクラス
    DerivedDerivedVirtualFixed ddvf{1};  // 上記クラスのコンストラクタを修正したクラス
    DerivedDerivedNormal ddn{1};         // 通常の継承クラスを継承したクラス

    ASSERT_EQ(0, ddv.get());  // 仮想継承独特の動作
    ASSERT_EQ(1, ddvf.get());
    ASSERT_EQ(1, ddn.get());
    // @@@ sample end
}
}  // namespace VirtualInheritance

namespace VirtualInheritanceWithDiamond {

// @@@ sample begin 1:0

int32_t base_called;

class Base {
public:
    explicit Base(int32_t x = 0) noexcept : x_{x} { ++base_called; }
    int32_t get() const noexcept { return x_; }

private:
    int32_t x_;
};

class Derived_0 : public virtual Base {  // 仮想継承
public:
    explicit Derived_0(int32_t x) noexcept : Base{x} { assert(base_called == 1); }
};

class Derived_1 : public virtual Base {  // 仮想継承
public:
    explicit Derived_1(int32_t x) noexcept : Base{x} { assert(base_called == 1); }
};

class DerivedDerived : public Derived_0, public Derived_1 {
public:
    DerivedDerived(int32_t x0, int32_t x1) noexcept : Derived_0{x0}, Derived_1{x1} {}
    // 「仮想継承のコンストラクタ呼び出し」仕様がなければ、このコンストラクタは、
    //    Base::Base -> Derived_0::Derived_0 ->
    //      Base::Base -> Derived_0::Derived_0 ->
    //        DerivedDerived::DerivedDerived
    //  という呼び出しになるため、Base::Baseが2度呼び出されてしまう。
};
// @@@ sample end

TEST(VirtualInheritance, virtual_inherit_diamond_0)
{
    // @@@ sample begin 1:1

    ASSERT_EQ(0, base_called);

    auto dd = DerivedDerived{2, 3};  // Base::Baseが最初に呼ばれないとassertion failする

    ASSERT_EQ(1, base_called);  // 「仮想継承のコンストラクタ呼び出し」仕様のため
    ASSERT_EQ(0, dd.get());  // Baseのデフォルトコンストラクタは、x_を0にする
    // @@@ sample end
}

namespace NG {
SUPPRESS_WARN_BEGIN;
SUPPRESS_WARN_CLANG_REORDER_CTOR;
SUPPRESS_WARN_GCC_REORDER;
// @@@ sample begin 2:0

class DerivedDerived : public Derived_0, public Derived_1 {
public:
    DerivedDerived(int32_t x0, int32_t x1) noexcept : Derived_0{x0}, Derived_1{x1}, Base{1} {}
};
// @@@ sample end
SUPPRESS_WARN_END;

TEST(VirtualInheritance, virtual_inherit_diamond_1)
{
    base_called = 0;
    // @@@ sample begin 2:1

    ASSERT_EQ(0, base_called);

    auto dd = DerivedDerived{2, 3};  // Base::Baseが最初に呼ばれないとassertion failする

    ASSERT_EQ(1, base_called);  // 「仮想継承のコンストラクタ呼び出し」仕様のため
    ASSERT_EQ(1, dd.get());  // Base{1}呼び出しの効果
    // @@@ sample end
}
}  // namespace NG

namespace OK {
// @@@ sample begin 3:0

class DerivedDerived : public Derived_0, public Derived_1 {
public:
    DerivedDerived(int32_t x0, int32_t x1) noexcept : Base{1}, Derived_0{x0}, Derived_1{x1} {}
};
// @@@ sample end

TEST(VirtualInheritance, virtual_inherit_diamond_2)
{
    base_called = 0;
    // @@@ sample begin 3:1
    ASSERT_EQ(0, base_called);

    auto dd = DerivedDerived{2, 3};

    ASSERT_EQ(1, base_called);
    ASSERT_EQ(1, dd.get());  // Base{1}呼び出しの効果
    // @@@ sample end
}
}  // namespace OK
}  // namespace VirtualInheritanceWithDiamond

#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace WithoutDiamond {

SUPPRESS_WARN_BEGIN;
SUPPRESS_WARN_CLANG_LITERAL_CONVERSION;
// @@@ sample begin 0:0

int32_t f(double) noexcept { return 0; }

struct Base {
    int32_t f(int32_t) const noexcept { return 1; }
    int32_t f(double) const noexcept { return 2; }
};

struct Derived : Base {
    int32_t f(int32_t) const noexcept { return 3; }  // Base::fを隠蔽する(name-hiding)
};

struct DerivedDerived : Derived {
    int32_t g() const noexcept { return f(2.14); }
};
// @@@ sample end

TEST(Dominance, without_diamond)
{
    // @@@ sample begin 0:1

    Base b;

    ASSERT_EQ(2, b.f(2.14));  // オーバーロード解決により、B::f(double)が呼ばれる

    DerivedDerived dd;

    // Derivedのドミナンスにより、B::fは、DerivedDerived::gでのfのname lookupの対象にならず、
    // DerivedDerived::gはDerived::fを呼び出す。
    ASSERT_EQ(3, dd.g());
    // @@@ sample end
}
}  // namespace WithoutDiamond

namespace WithDiamond {
// @@@ sample begin 1:0

struct Base {
    int32_t f(int32_t) const noexcept { return 1; }
    int32_t f(double) const noexcept { return 2; }
};

struct Derived_0 : Base {
    int32_t f(int32_t) const noexcept { return 3; }  // Base::fを隠蔽する(name-hiding)
};

struct Derived_1 : Base {};

#if 0   // @@@ delete
struct DerivedDerived : Derived_0, Derived_1 {
    int32_t g() const noexcept { return f(2.14); }  // Derived_0::f or Derived_1::f ?
};

// dominance_ut.cpp:58:41: error: reference to ‘f’ is ambiguous
//    58 |     int32_t g() const noexcept { return f(2.14); }  // Derived_0::f or Derived_1::f ?
//       |                                         ^
#endif  // @@@ delete
// @@@ sample end
}  // namespace WithDiamond

namespace WithVirtualDiamond {
// @@@ sample begin 2:0

struct Base {
    int32_t f(int32_t) const noexcept { return 1; }
    int32_t f(double) const noexcept { return 2; }
};

struct Derived_0 : virtual Base {
    int32_t f(int32_t) const noexcept { return 3; }  // Base::fを隠蔽する(name-hiding)
};

struct Derived_1 : virtual Base {};

struct DerivedDerived : Derived_0, Derived_1 {
    int32_t g() const noexcept { return f(2.14); }
};
// @@@ sample end
SUPPRESS_WARN_END;

TEST(Dominance, with_virtual_diamond)
{
    // @@@ sample begin 2:1

    DerivedDerived dd;

    // Derived_0のドミナンスと仮想継承の効果により、
    // B::fは、DerivedDerived::gでのfのname lookupの対象にならず、
    // DerivedDerived::gはDerived_0::fを呼び出す。
    ASSERT_EQ(3, dd.g());
    // @@@ sample end
}
}  // namespace WithVirtualDiamond

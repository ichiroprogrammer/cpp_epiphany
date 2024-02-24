#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace {

SUPPRESS_WARN_BEGIN;
SUPPRESS_WARN_GCC_NOT_EFF_CPP;
// @@@ sample begin 0:0

bool X_destructed;
class X {
public:
    X() { X_destructed = false; }
    ~X() { X_destructed = true; }
};

bool A_destructed;
class A {
public:
    A() { A_destructed = false; }
    ~A() { A_destructed = true; }

    X const& GetX() const noexcept { return x_; }

private:
    X x_;
};
// @@@ sample end
SUPPRESS_WARN_END;

TEST(TermExplanationMisra, dangling_ref)
{
    // @@@ sample begin 0:1

    auto a = A{};

    auto const& x_safe = a.GetX();  // x_safeはダングリングリファレンスではない
    ASSERT_FALSE(A_destructed || X_destructed);

    auto const& x_dangling = A{}.GetX();  // 次の行でxが指すオブジェクトは解放される
    // この行ではxはdangngling リファレンスになる。
    ASSERT_TRUE(A_destructed && X_destructed);

    auto const* x_ptr_dangling = &A{}.GetX();  // 次の行でxが指すオブジェクトは解放される
    // この行ではxはdangngling ポインタになる。
    ASSERT_TRUE(A_destructed && X_destructed);
    // @@@ sample end

    IGNORE_UNUSED_VAR(x_safe, x_dangling, x_ptr_dangling);
}
}  // namespace

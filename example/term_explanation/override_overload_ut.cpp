#include "gtest_wrapper.h"

namespace OverrideSample {

// @@@ sample begin 0:0

class Base {
public:
    virtual ~Base() = default;
    virtual std::string f() { return "Base::f"; }
    std::string         g() { return "Base::g"; }

    // g()のオーバーロード
    std::string g(int) { return "Base::g(int)"; }
};

class Derived : public Base {
public:
    // Base::fのオーバーライド
    virtual std::string f() override { return "Derived::f"; }

    // Base::gのname-hiding
    std::string g() { return "Derived::g"; }
};
// @@@ sample end

TEST(OverrideOverload, override_overload)
{
    // @@@ sample begin 0:1

    auto  ret   = std::string{};
    auto  b     = Base{};
    auto  d     = Derived{};
    Base& d_ref = d;

    ret = b.f();  // Base::f()呼び出し
    ASSERT_EQ("Base::f", ret);

    ret = d.f();  // Derived::f()呼び出し
    ASSERT_EQ("Derived::f", ret);

    ret = b.g();  // Base::g()呼び出し
    ASSERT_EQ("Base::g", ret);

    ret = d.g();  // Derived::g()呼び出し
    ASSERT_EQ("Derived::g", ret);
    // ret = d.g(int{});   // Derived::gによって、Base::gが隠されるのでコンパイルエラー

    ret = d_ref.f();  // Base::fはDerived::fによってオーバーライドされたので、Derived::f()呼び出し
    ASSERT_EQ("Derived::f", ret);

    ret = d_ref.g();  // d_refの表層型はBaseなので、Base::g()呼び出し
    ASSERT_EQ("Base::g", ret);

    ret = d_ref.g(int{});  // d_refの表層型はBaseなので、Base::g(int)呼び出し
    ASSERT_EQ("Base::g(int)", ret);
    // @@@ sample end
}
}  // namespace OverrideSample

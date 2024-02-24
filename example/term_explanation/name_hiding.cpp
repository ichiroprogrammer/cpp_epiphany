#include "gtest_wrapper.h"

namespace NameHidingDerived {
// @@@ sample begin 0:0

struct Base {
    void f() noexcept {}
};

struct Derived : Base {
    // void f(int) { f(); }     // f()では、Baseのf()をname lookupできないため、
    void f(int) noexcept { Base::f(); }  // Base::でf()を修飾した
};
// @@@ sample end

namespace {
TEST(NameHiding, derived)
// @@@ sample begin 0:1

{
    auto d = Derived{};
#if 0 
    d.f(); // コンパイルできない
#else
    d.Base::f();  // Base::での修飾が必要
#endif
}
// @@@ sample end
}  // namespace
}  // namespace NameHidingDerived
namespace NameHidingDerivedFixed {
using NameHidingDerived::Base;

// @@@ sample begin 1:0

struct Derived : Base {
    using Base::f;  // using宣言によりDerivedにBase::fを導入
    void f(int) noexcept { Base::f(); }
};
// @@@ sample end

namespace {
TEST(NameHiding, derived_fixed)
{
    // @@@ sample begin 1:1

    auto d = Derived{};
    d.f();  // using宣言によりコンパイルできる
    // @@@ sample end
}
}  // namespace
}  // namespace NameHidingDerivedFixed

// @@@ sample begin 2:0

// global名前空間
void f() noexcept {}

namespace NS_A {
void f(int) noexcept {}

void g() noexcept
{
#if 0
    f();  // NS_A::fによりname-hidingされたため、コンパイルできない
#endif
}
}  // namespace NS_A
// @@@ sample end
// @@@ sample begin 2:1

namespace NS_A_fixed_0 {
void g() noexcept
{
    // グローバルなfの呼び出し
    f();  // NS_A::fは後方に移動されたためコンパイルできる
}

void f(int) noexcept {}
}  // namespace NS_A_fixed_0
// @@@ sample end
// @@@ sample begin 2:2

namespace NS_A_fixed_1 {
void f(int) noexcept {}

void g() noexcept
{
    using ::f;

    // グローバルなfの呼び出し
    f();  // using宣言によりコンパイルできる
}
}  // namespace NS_A_fixed_1
// @@@ sample end
// @@@ sample begin 2:3

namespace NS_A_fixed_2 {
void f(int) noexcept {}

void g() noexcept
{
    // グローバルなfの呼び出し
    ::f();  // ::で修飾すればコンパイルできる
}
}  // namespace NS_A_fixed_2
// @@@ sample end
// @@@ sample begin 3:0

namespace NS_B {
struct S_in_B {};

void f(S_in_B) noexcept {}
void f(int) noexcept {}

namespace NS_B_Inner {
void g() noexcept
{
    f(int{});  // コンパイルでき、NS_B::f(int)が呼ばれる
}

void f() noexcept {}

void h() noexcept
{
    // f(int{});     // コンパイルできない
    NS_B::f(int{});  // 名前空間で修飾することでコンパイルできる

    f(S_in_B{});  // ADLによりコンパイルできる
}
}  // namespace NS_B_Inner
}  // namespace NS_B
// @@@ sample end

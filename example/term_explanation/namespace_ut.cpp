#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace NamespaceSample {
// @@@ sample begin 0:0
namespace XXX {
void func() noexcept {}
void gunc() noexcept {}
}  // namespace XXX
// @@@ sample end
// @@@ sample begin 0:1

// global namespace
void using_declaration() noexcept
{
    using XXX::func;  // using宣言

    func();       // XXX::不要
    XXX::gunc();  // XXX::必要
}

// @@@ sample end
// @@@ sample begin 0:2

// global namespace
void using_directive() noexcept
{
    using namespace XXX;  // usingディレクティブ

    func();  // XXX::不要
    gunc();  // XXX::不要
}
// @@@ sample end
// @@@ sample begin 0:3

namespace XXX_Inner {
void func(int) noexcept {}
void using_declaration() noexcept
{
#if 0
    using namespace XXX;  // name-hidingのため効果がない
#else
    using XXX::func;  // using宣言
#endif

    func();  // XXX::不要
}
// @@@ sample end
// @@@ sample end
}  // namespace XXX_Inner

namespace {
TEST(Namespace, adl)
{
    // 下記operator <<は、std::operator<<(ostream&, string const&)であり、
    // namespace stdで定義されている。

    // 実引数依存探索があるため、operator <<は名前空間修飾無しで呼び出せる。
    std::cout << std::string{__func__};

    // 実引数依存探索が無いと下記のような呼び出しになる。
    std::operator<<(std::cout, std::string{__func__});
}
}  // namespace
}  // namespace NamespaceSample

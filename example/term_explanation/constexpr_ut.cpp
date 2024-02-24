#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace ConstexprFunctionSample {
// @@@ sample begin 0:0

constexpr double PI{3.14159265358979323846};  // PIはconstexpr
// @@@ sample end
// @@@ sample begin 0:1

constexpr int f(int a) noexcept { return a * 3; }  // aがconstexprならばf(a)もconstexpr

int g(int a) noexcept { return a * 3; }  // aがconstexprであってもg(a)は非constexpr

template <int N>
struct Templ {
    static constexpr auto value = N;
};
// @@@ sample end

namespace {
TEST(Constexpr, constexpr_var)
{
    // @@@ sample begin 0:2

    auto x = int{0};

    constexpr auto a = f(3);  // f(3)はconstexprなのでaの初期化が可能
    // constexpr auto b = f(x);  // xは非constexprなのでbの初期化はコンパイルエラー
    auto const c = f(3);  // cはconstexprとすべき
    // constexpr auto d = g(3);  // g(3)は非constexprなのでdの初期化はコンパイルエラー
    auto const e = g(x);  // eはここで初期化して、この後不変

    constexpr auto pi = PI;  // PIもconstexprなので初期化が可能

    auto templ_a = Templ<a>{};     // aはconstexprなのでaの初期化が可能
    auto templ_f = Templ<f(a)>{};  // f(a)はconstexprなのでaの初期化が可能
    // auto templ_x = Templ<x>{};  // xは非constexprなのでテンプレートパラメータに指定できない

    static_assert(templ_a.value == 9);
    // static_assert(x == 0);  // xは非constexprなのでstatic_assertで使用できない
    // @@@ sample end

    IGNORE_UNUSED_VAR(x, a, c, e, pi, templ_a, templ_f);
}
}  // namespace
// @@@ sample begin 0:3

inline constexpr uint32_t BitMask(uint32_t bit_len) noexcept
{
    if (bit_len == 0) {
        return 0x0;
    }

    return BitMask(bit_len - 1) | (0x01 << (bit_len - 1));
}
// @@@ sample end

namespace {
TEST(Constexpr, BitMask)
{
    // @@@ sample begin 0:4

    constexpr auto b_0x00000000 = BitMask(0);
    constexpr auto b_0x000000ff = BitMask(8);

    static_assert(b_0x00000000 == 0x00000000);
    static_assert(b_0x000000ff == 0x000000ff);
    static_assert(BitMask(16) == 0x0000'ffff);

    constexpr auto bit_len_constexpr = 24U;
    static_assert(BitMask(bit_len_constexpr) == 0x00ff'ffff);

    auto bit_len = 24U;

    // bit_lenがconstexprでないことによりBitMask(bit_len)もconstexprでないため、
    // コンパイルできない
    // constexpr auto b_0x00ffffff = BitMask(bit_len);

    // b_0x00ffffffの定義からconstexprを外せばコンパイル可能
    // ただし、コンパイル時でなくランタイム時動作になるため動作が遅い
    auto b_0x00ffffff = BitMask(bit_len);

    ASSERT_EQ(b_0x00ffffff, 0x00ff'ffff);
    // @@@ sample end
}
}  // namespace

// @@@ sample begin 0:5

class Integer {
public:
    constexpr Integer(int32_t integer) noexcept : integer_{integer} {}
    constexpr int32_t Get() const noexcept { return integer_; }  // constexprメンバ関数はconst
    constexpr int32_t Allways2() const noexcept { return 2; }    // constexprメンバ関数はconst
    static constexpr int32_t Allways3() noexcept { return 3; }   // static関数のconstexpr化

private:
    int32_t integer_;
};
// @@@ sample end

namespace {
TEST(Constexpr, constexpr_class)
{
    // @@@ sample begin 0:6

    constexpr auto int3     = 3;                         // int3はconstexpr
    constexpr auto integer3 = Integer{int3};             // integer3自体がconstexpr
    static_assert(integer3.Get() == 3, "wrong number");  // integer3.Get()もconstexpr

    auto integer4 = Integer{4};
    // integer4は非constexprであるため、integer4.Get()も非constexprとなり、コンパイル不可
    // static_assert(integer4.Get() == 4, "wrong number");

    // integer4は非constexprだが、integer4.Allway2()はconstexprであるため、コンパイル可能
    static_assert(integer4.Allways2() == 2, "wrong number");
    // @@@ sample end
}
}  // namespace
}  // namespace ConstexprFunctionSample

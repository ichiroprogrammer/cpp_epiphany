#include <cstdint>

#include "gtest_wrapper.h"

namespace {
// clang-format off
// @@@ sample begin 0:0

template <typename T> constexpr int32_t f(T) noexcept { return 0; }                   // f-0
template <typename T> constexpr int32_t f(T*) noexcept { return 1; }                  // f-1
template <>           constexpr int32_t f<int32_t*>(int32_t*) noexcept { return 2; }  // f-2
// f-2はf-1の特殊化のように見えるが、T == int32_t*の場合のf-0の特殊化である。
// @@@ sample end
// clang-format on

TEST(Template, template_overload)
{
    // @@@ sample begin 0:1

    // 以下、f-0/f-1/f-2のテスト
    auto c   = char{0};
    auto i32 = 0;

    // 以下はおそらく直感通り
    static_assert(f(0) == 0);               // f-0が呼ばれる
    static_assert(f(&c) == 1);              // f-1が呼ばれる
    static_assert(f<int32_t*>(&i32) == 2);  // f-2が呼ばれる

    // 以下はおそらく直感に反する
    static_assert(f(nullptr) == 0);  // f-1ではなく、f-0が呼ばれる
    static_assert(f(&i32) == 1);     // f-2ではなく、f-1が呼ばれる
    // @@@ sample end
}
}  // namespace

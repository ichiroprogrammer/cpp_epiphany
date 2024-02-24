#include <iterator>
#include <type_traits>

#include "gtest_wrapper.h"

namespace NotConstexprIf {
namespace {

// @@@ sample begin 0:0

// 配列のサイズ
template <typename T>
auto Length(T const&) -> std::enable_if_t<std::is_array_v<T>, size_t>
{
    return std::extent_v<T>;
}

// コンテナのサイズ
template <typename T>
auto Length(T const& t) -> decltype(t.size())
{
    return t.size();
}

// その他のサイズ
size_t Length(...) { return 0; }
// @@@ sample end

TEST(Template, constexpr_if)
{
    // @@@ sample begin 0:1

    uint32_t a[5];
    auto     v = std::vector{0, 1, 2};
    struct SizeTest {
    } t;

    ASSERT_EQ(5, Length(a));
    ASSERT_EQ(3, Length(v));
    ASSERT_EQ(0, Length(t));

    // C++17で、Lengthと同様の機能の関数テンプレートがSTLに追加された
    ASSERT_EQ(std::size(a), Length(a));
    ASSERT_EQ(std::size(v), Length(v));
    // @@@ sample end
}
}  // namespace
}  // namespace NotConstexprIf

namespace ConstexprIf {
namespace {
// @@@ sample begin 0:2

struct helper {
    template <typename T>
    auto operator()(T const& t) -> decltype(t.size());
};

template <typename T>
size_t Length(T const& t)
{
    if constexpr (std::is_array_v<T>) {  // Tが配列の場合
        // Tが配列でない場合、他の条件のブロックはコンパイル対象外
        return std::extent_v<T>;
    }
    else if constexpr (std::is_invocable_v<helper, T>) {  // T::Lengthが呼び出せる場合
        // T::Lengthが呼び出せない場合、他の条件のブロックはコンパイル対象外
        return t.size();
    }
    else {  // それ以外
        // Tが配列でなく且つ、T::Lengthが呼び出ない場合、他の条件のブロックはコンパイル対象外
        return 0;
    }
}
// @@@ sample end

TEST(Template, constexpr_if_size)
{
    uint32_t             a[5];
    std::vector<int32_t> v{0, 1, 2};
    struct SizeTest {
    } t;

    ASSERT_EQ(5, Length(a));
    ASSERT_EQ(3, Length(v));
    ASSERT_EQ(0, Length(t));
}
}  // namespace
}  // namespace ConstexprIf

namespace NotConstexprIf {
namespace {
// @@@ sample begin 1:0

// テンプレートパラメータで与えられた型のsizeofの値が最も大きな値を返す。
template <typename HEAD>
constexpr size_t MaxSizeof()
{
    return sizeof(HEAD);
}

template <typename HEAD, typename T, typename... TAILS>
constexpr size_t MaxSizeof()
{
    return std::max(sizeof(HEAD), MaxSizeof<T, TAILS...>());
}
// @@@ sample end

TEST(Template, constexpr_if_sum)
{
    // @@@ sample begin 1:1

    static_assert(4 == (MaxSizeof<int8_t, int16_t, int32_t>()));
    static_assert(4 == (MaxSizeof<int32_t, int16_t, int8_t>()));
    static_assert(sizeof(std::string) == MaxSizeof<int32_t, int16_t, int8_t, std::string>());
    // @@@ sample end
}
}  // namespace
}  // namespace NotConstexprIf
namespace ConstexprIf {
namespace {

// @@@ sample begin 1:2

// テンプレートパラメータで与えられた型のsizeofの値が最も大きな値を返す。
template <typename HEAD, typename... TAILS>
constexpr size_t MaxSizeof()
{
    if constexpr (sizeof...(TAILS) == 0) {  // TAILSが存在しない場合
        return sizeof(HEAD);
    }
    else {
        return std::max(sizeof(HEAD), MaxSizeof<TAILS...>());
    }
}
// @@@ sample end

TEST(Template, constexpr_if_sum)
{
    static_assert(4 == (MaxSizeof<int8_t, int16_t, int32_t>()));
    static_assert(4 == (MaxSizeof<int32_t, int16_t, int8_t>()));
    static_assert(sizeof(std::string) == MaxSizeof<int32_t, int16_t, int8_t, std::string>());
}
}  // namespace
}  // namespace ConstexprIf

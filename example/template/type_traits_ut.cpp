#include <type_traits>
#include <vector>

#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace {
namespace TypeTraits {

TEST(Template, true_false_type)
{
    // @@@ sample begin 0:0

    // std::is_same_vの2パラメータが同一であれば、std::is_same_v<> == true
    static_assert(std::is_same_v<std::integral_constant<bool, true>, std::true_type>);
    static_assert(std::is_same_v<std::integral_constant<bool, false>, std::false_type>);
    // @@@ sample end

    // @@@ sample begin 0:1

    static_assert(std::true_type::value, "must be true");
    static_assert(!std::false_type::value, "must be false");
    // @@@ sample end
}

SUPPRESS_WARN_BEGIN;
SUPPRESS_WARN_UNUSED_FUNC;
// @@@ sample begin 0:2

// 引数の型がintに変換できるかどうかを判定する関数
// decltypeの中でのみ使用されるため、定義は不要
constexpr std::true_type  IsCovertibleToInt(int);  // intに変換できる型はこちら
constexpr std::false_type IsCovertibleToInt(...);  // それ以外はこちら
// @@@ sample end
SUPPRESS_WARN_BEGIN;

TEST(Template, true_false_type2)
{
    // @@@ sample begin 0:3

    static_assert(decltype(IsCovertibleToInt(1))::value);
    static_assert(decltype(IsCovertibleToInt(1u))::value);
    static_assert(!decltype(IsCovertibleToInt(""))::value);  // ポインタはintに変換不可

    struct ConvertibleToInt {
        operator int();
    };

    struct NotConvertibleToInt {};

    static_assert(decltype(IsCovertibleToInt(ConvertibleToInt{}))::value);
    static_assert(!decltype(IsCovertibleToInt(NotConvertibleToInt{}))::value);

    // なお、IsCovertibleToInt()やConvertibleToInt::operator int()は実際に呼び出されるわけでは
    // ないため、定義は必要なく宣言のみがあれば良い。
    // @@@ sample end
}

TEST(Template, type_traits_integral_constant)
{
    // @@@ sample begin 1:0

    using int3 = std::integral_constant<int, 3>;

    // std::is_same_vの2パラメータが同一であれば、std::is_same_v<> == true
    static_assert(std::is_same_v<int, int3::value_type>);
    static_assert(std::is_same_v<std::integral_constant<int, 3>, int3::type>);
    static_assert(int3::value == 3);

    using bool_true = std::integral_constant<bool, true>;

    static_assert(std::is_same_v<bool, bool_true::value_type>);
    static_assert(std::is_same_v<std::integral_constant<bool, true>, bool_true::type>);
    static_assert(bool_true::value == true);
    // @@@ sample end
}

TEST(Template, type_traits_is_void)
{
    // ここは、md/template_meta_programming.mdの都合上is_void<>::valueを使う
    // @@@ sample begin 2:0

    static_assert(std::is_void<void>::value);
    static_assert(!std::is_void<int>::value);
    static_assert(!std::is_void<std::string>::value);
    // @@@ sample end
}

// @@@ sample begin 3:1

template <typename T, typename U>
constexpr bool is_same_v{std::is_same<T, U>::value};
// @@@ sample end

TEST(Template, type_traits_is_same)
{
    // ここは、md/template_meta_programming.mdの都合上is_same<>::valueを使う
    // @@@ sample begin 3:0

    static_assert(std::is_same<int, int>::value);
    static_assert(std::is_same<int, int32_t>::value);   // 64ビットg++/clang++
    static_assert(!std::is_same<int, int64_t>::value);  // 64ビットg++/clang++
    static_assert(std::is_same<std::string, std::basic_string<char>>::value);
    static_assert(std::is_same<typename std::vector<int>::reference, int&>::value);
    // @@@ sample end

    // @@@ sample begin 3:2

    static_assert(is_same_v<int, int>);
    static_assert(is_same_v<int, int32_t>);   // 64ビットg++/clang++
    static_assert(!is_same_v<int, int64_t>);  // 64ビットg++/clang++
    static_assert(is_same_v<std::string, std::basic_string<char>>);
    static_assert(is_same_v<typename std::vector<int>::reference, int&>);
    // @@@ sample end

    // @@@ sample begin 3:3

    static_assert(std::is_base_of_v<std::true_type, std::is_same<int, int>>);
    static_assert(std::is_base_of_v<std::false_type, std::is_same<int, char>>);
    // @@@ sample end
}

// @@@ sample begin 4:0

template <bool T_F, typename T = void>
struct enable_if;

template <typename T>
struct enable_if<true, T> {
    using type = T;
};

template <typename T>
struct enable_if<false, T> {  // メンバエイリアスtypeを持たない
};

template <bool COND, typename T = void>
using enable_if_t = typename enable_if<COND, T>::type;
// @@@ sample end

TEST(Template, type_traits_enable_if)
{
    // 自作enable_ifのテスト
    static_assert(std::is_same_v<void, enable_if_t<true>>);
    static_assert(std::is_same_v<int, enable_if_t<true, int>>);

    // @@@ sample begin 4:1

    static_assert(std::is_same_v<void, std::enable_if_t<true>>);
    static_assert(std::is_same_v<int, std::enable_if_t<true, int>>);
    // @@@ sample end

#if 0
    // @@@ sample begin 4:2

    // 下記はill-formedとなるため、コンパイルできない。
    static_assert(std::is_same_v<void, std::enable_if_t<false>>);
    static_assert(std::is_same_v<int, std::enable_if_t<false, int>>);
    // @@@ sample end
#endif
}

// @@@ sample begin 5:0

template <bool T_F, typename, typename>
struct conditional;

template <typename T, typename U>
struct conditional<true, T, U> {
    using type = T;
};

template <typename T, typename U>
struct conditional<false, T, U> {
    using type = U;
};

template <bool COND, typename T, typename U>
using conditional_t = typename conditional<COND, T, U>::type;
// @@@ sample end

TEST(Template, type_traits_conditional)
{
    // 自作conditionalのテスト
    static_assert(std::is_same_v<int, conditional_t<true, int, char>>);
    static_assert(std::is_same_v<char, conditional_t<false, int, char>>);

    // @@@ sample begin 5:1

    static_assert(std::is_same_v<int, std::conditional_t<true, int, char>>);
    static_assert(std::is_same_v<char, std::conditional_t<false, int, char>>);
    // @@@ sample end
}
}  // namespace TypeTraits
}  // namespace

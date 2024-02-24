#include <type_traits>
#include <vector>

#include "gtest_wrapper.h"

#include "nstd_type_traits.h"
#include "suppress_warning.h"
#include "test_class.h"

namespace {
namespace ExistsMemberFunc {

// @@@ sample begin 0:0

namespace Inner_ {

template <typename T>
using exists_void_func_void =
    // @@@ sample end
    // @@@ sample begin 0:1
    // メンバvoid func(void)があれば、voidを生成
    // メンバvoid func(void)がなければ、ill-formed
    typename std::enable_if_t<std::is_same_v<decltype(std::declval<T>().func()), void>>;
// @@@ sample end
// @@@ sample begin 0:2
}  // namespace Inner_

template <typename T, typename = Inner_::exists_void_func_void<T>>
constexpr bool exists_void_func_sfinae_f(T) noexcept
{
    return true;
}

constexpr bool exists_void_func_sfinae_f(...) noexcept { return false; }
// @@@ sample end

SUPPRESS_WARN_BEGIN;
SUPPRESS_WARN_CLANG_UNDEFINED_INTERNAL;

// @@@ sample begin 0:3

// テスト用クラス
struct X {
    void func();
};

struct Y {
    int func();
};

struct Z {
private:
    void func();  // privateなvoid func()は外部からは呼び出せない
};
// @@@ sample end
SUPPRESS_WARN_END;

TEST(Template, exists_void_func_sfinae_f)
{
    // @@@ sample begin 0:4

    static_assert(!exists_void_func_sfinae_f(int{}));
    static_assert(exists_void_func_sfinae_f(X{}));
    static_assert(!exists_void_func_sfinae_f(Y{}));
    static_assert(!exists_void_func_sfinae_f(Z{}));
    // @@@ sample end
}

// @@@ sample begin 1:0

template <typename T, typename U = void>
struct exists_void_func_sfinae_s : std::false_type {
};

// clang-format off
template <typename T>
struct exists_void_func_sfinae_s<T,
    // メンバvoid func()が呼び出せれば、voidを生成
    // メンバvoid func()が呼び出せなければ、ill-formed
    decltype(std::declval<T>().func())
    > : std::true_type {
};
// clang-format on

template <typename T>
constexpr bool exists_void_func_sfinae_s_v{exists_void_func_sfinae_s<T>::value};
// @@@ sample end

TEST(Template, exists_void_func_sfinae_s)
{
    // @@@ sample begin 1:1

    static_assert(!exists_void_func_sfinae_s_v<int>);
    static_assert(exists_void_func_sfinae_s_v<X>);
    static_assert(!exists_void_func_sfinae_s_v<Y>);
    static_assert(!exists_void_func_sfinae_s_v<Z>);
    // @@@ sample end
}

// @@@ sample begin 2:0

template <typename T>
class exists_void_func_sfinae_s2 {
    // @@@ sample end
    // @@@ sample begin 2:1

    // メンバvoid func()が呼び出せれば、メンバ関数テンプレートはtrueを返す
    // メンバvoid func()が呼び出せなければ、ill-formed
    template <typename U, void (U::*)() = &U::func>
    static constexpr bool detector(U*) noexcept
    {
        return true;
    }
    // @@@ sample end
    // @@@ sample begin 2:2

    static constexpr bool detector(...) noexcept { return false; }

public:
    static constexpr bool value{exists_void_func_sfinae_s2::detector(static_cast<T*>(nullptr))};
};

template <typename T>
constexpr bool exists_void_func_sfinae_s2_v{exists_void_func_sfinae_s2<T>::value};
// @@@ sample end

TEST(Template, exists_void_func_sfinae_s2)
{
    // @@@ sample begin 2:3

    static_assert(!exists_void_func_sfinae_s2_v<int>);
    static_assert(exists_void_func_sfinae_s2_v<X>);
    static_assert(!exists_void_func_sfinae_s2_v<Y>);
    static_assert(!exists_void_func_sfinae_s2_v<Z>);
    // @@@ sample end
}
}  // namespace ExistsMemberFunc

namespace IsRange_C0 {
// @@@ sample begin 3:0

template <typename, typename = void>
struct exists_begin : std::false_type {
};

template <typename T>
struct exists_begin<T, std::void_t<decltype(std::begin(std::declval<T>()))>> : std::true_type {
};

template <typename T>
constexpr bool exists_begin_v{exists_begin<T>::value};
// @@@ sample end

TEST(Template, exists_begin)
{
    // @@@ sample begin 3:1

    static_assert(exists_begin_v<std::string>);
    static_assert(!exists_begin_v<int>);
    static_assert(exists_begin_v<int const[3]>);
    // @@@ sample end

#if 1
    static_assert(!exists_begin_v<int[3]>);
#else
    // @@@ sample begin 3:2

    // 以下が問題
    static_assert(exists_begin_v<int[3]>);
    // @@@ sample end
#endif

    static_assert(std::is_same_v<int(&&)[3], decltype(std::declval<int[3]>())>);

    int   i3[3];
    auto& r = i3;
    static_assert(std::is_same_v<int(&)[3], decltype(r)>);
}
}  // namespace IsRange_C0

namespace IsRange_C1 {

// @@@ sample begin 4:0

template <typename, typename = void>
struct exists_begin : std::false_type {
};

// Tが非配列の場合の特殊化
template <typename T>
struct exists_begin<T,
                    typename std::enable_if_t<!std::is_array_v<T>,
                                              std::void_t<decltype(std::begin(std::declval<T>()))>>>
    : std::true_type {
};

// Tが配列の場合の特殊化
template <typename T>
struct exists_begin<T, typename std::enable_if_t<std::is_array_v<T>>> : std::true_type {
};

template <typename T>
constexpr bool exists_begin_v{exists_begin<T>::value};
// @@@ sample end

TEST(Template, exists_begin)
{
    static_assert(exists_begin_v<std::string>);
    static_assert(!exists_begin_v<int>);
    static_assert(exists_begin_v<int const[3]>);

    // 問題が解決
    static_assert(exists_begin_v<int[3]>);
}
}  // namespace IsRange_C1

namespace Complicated_ExistsPutto {
namespace Inner_ {
// @@@ sample begin 5:0

template <typename, typename = std::ostream&>
struct exists_put_to_as_member : std::false_type {
};

template <typename T>
struct exists_put_to_as_member<T, decltype(std::declval<std::ostream&>().operator<<(
                                      std::declval<T>()))> : std::true_type {
};

template <typename T>
constexpr bool exists_put_to_as_member_v{exists_put_to_as_member<T>::value};
// @@@ sample end

TEST(Template, exists_put_to_as_member)
{
    // @@@ sample begin 5:1

    static_assert(exists_put_to_as_member_v<bool>);
    static_assert(!exists_put_to_as_member_v<std::string>);
    static_assert(!exists_put_to_as_member_v<std::vector<int>>);
    static_assert(exists_put_to_as_member_v<std::vector<int>*>);
    static_assert(!exists_put_to_as_member_v<test_class_exits_put_to>);
    static_assert(!exists_put_to_as_member_v<test_class_not_exits_put_to>);
    // @@@ sample end
    // @@@ sample begin 5:2
    static_assert(exists_put_to_as_member_v<test_class_not_exits_put_to[3]>);  // 驚き!
    // @@@ sample end

    // to suppress warning by clang
    auto oss = std::ostringstream{};
    oss << test_class_exits_put_to{};
}

// @@@ sample begin 5:3

template <typename, typename = std::ostream&>
struct exists_put_to_as_non_member : std::false_type {
};

template <typename T>
struct exists_put_to_as_non_member<T, decltype(operator<<(std::declval<std::ostream&>(),
                                                          std::declval<T>()))> : std::true_type {
};

template <typename T>
constexpr bool exists_put_to_as_non_member_v{exists_put_to_as_non_member<T>::value};
// @@@ sample end

TEST(Template, exists_put_to_as_non_member)
{
    using namespace Inner_;

    static_assert(!exists_put_to_as_non_member_v<bool>);
    static_assert(exists_put_to_as_non_member_v<std::string>);
    static_assert(!exists_put_to_as_non_member_v<std::vector<int>>);
    static_assert(!exists_put_to_as_non_member_v<std::vector<int>*>);
    static_assert(exists_put_to_as_non_member_v<test_class_exits_put_to>);
    static_assert(!exists_put_to_as_non_member_v<test_class_not_exits_put_to>);
    static_assert(!exists_put_to_as_non_member_v<test_class_not_exits_put_to[3]>);
}
}  // namespace Inner_

// @@@ sample begin 5:4

template <typename T>
struct ExistsPutTo
    : std::conditional_t<
          Inner_::exists_put_to_as_member_v<T> || Inner_::exists_put_to_as_non_member_v<T>,
          std::true_type, std::false_type> {
};

template <typename T>
constexpr bool ExistsPutToV{ExistsPutTo<T>::value};
// @@@ sample end

TEST(Template, exists_put_to)
{
    using namespace Inner_;

    static_assert(ExistsPutToV<bool>);
    static_assert(ExistsPutToV<std::string>);
    static_assert(!ExistsPutToV<std::vector<int>>);
    static_assert(ExistsPutToV<std::vector<int>*>);
    static_assert(ExistsPutToV<test_class_exits_put_to>);
    static_assert(!ExistsPutToV<test_class_not_exits_put_to>);
    static_assert(ExistsPutToV<test_class_not_exits_put_to[3]>);
}
}  // namespace Complicated_ExistsPutto
}  // namespace

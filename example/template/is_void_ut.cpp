#include <type_traits>

#include "gtest_wrapper.h"

namespace {
namespace IsVoidXXX {
// @@@ sample begin 0:0

template <typename T>
constexpr bool is_void_f() noexcept
{
    return false;
}

template <>
constexpr bool is_void_f<void>() noexcept
{
    return true;
}

template <typename T>
constexpr bool is_void_f_v{is_void_f<T>()};
// @@@ sample end

TEST(Template, type_traits_is_void_f)
{
    // @@@ sample begin 0:1

    static_assert(!is_void_f_v<int>);
    static_assert(!is_void_f_v<std::string>);
    static_assert(is_void_f_v<void>);
    // @@@ sample end
}

// @@@ sample begin 1:0

template <typename T>
struct is_void_s {
    static constexpr bool value{false};
};

template <>
struct is_void_s<void> {
    static constexpr bool value{true};
};

template <typename T>
constexpr bool is_void_s_v{is_void_s<T>::value};
// @@@ sample end

TEST(Template, type_traits_is_void_s)
{
    // @@@ sample begin 1:2

    static_assert(!is_void_s_v<int>);
    static_assert(!is_void_s_v<std::string>);
    static_assert(is_void_s_v<void>);
    // @@@ sample end
}

// @@@ sample begin 2:0

namespace Inner_ {

// T == void
template <typename T>
constexpr auto is_void_sfinae_f_detector(void const* v, T const* t) noexcept
    -> decltype(t = v, bool{})  // T != voidの場合、t = vはill-formed
                                // T == voidの場合、well-formedでbool型生成
{
    return true;
}

// T != void
template <typename T>
constexpr auto is_void_sfinae_f_detector(void const*, T const*) noexcept
    -> decltype(sizeof(T), bool{})  // T != voidの場合、well-formedでbool型生成
                                    // T == voidの場合、sizeof(T)はill-formed
{
    return false;
}
}  // namespace Inner_

template <typename T>
constexpr bool is_void_sfinae_f() noexcept
{
    return Inner_::is_void_sfinae_f_detector(nullptr, static_cast<T*>(nullptr));
}

template <typename T>
constexpr bool is_void_sfinae_f_v{is_void_sfinae_f<T>()};
// @@@ sample end

TEST(Template, type_traits_is_void_sfinae_f)
{
    // @@@ sample begin 2:1

    static_assert(!is_void_sfinae_f_v<int>);
    static_assert(!is_void_sfinae_f_v<std::string>);
    static_assert(is_void_sfinae_f_v<void>);
    // @@@ sample end
}

namespace SecondaryImple {
// @@@ sample begin 3:0

template <typename T>
class is_void_sfinae_f {
    // U == void
    template <typename U>
    static constexpr auto detector(void const* v, U const* u) noexcept
        -> decltype(u = v, bool{})  // U != voidの場合、t = vはill-formed
                                    // U == voidの場合、well-formedでbool型生成
    {
        return true;
    }

    // U != void
    template <typename U>
    static constexpr auto detector(void const*, U const*) noexcept
        -> decltype(sizeof(U), bool{})  // U != voidの場合、well-formedでbool型生成
                                        // U == voidの場合、ill-formed
    {
        return false;
    }

public:
    static constexpr bool value{is_void_sfinae_f::detector(nullptr, static_cast<T*>(nullptr))};
};

template <typename T>
constexpr bool is_void_sfinae_f_v{is_void_sfinae_f<T>::value};
// @@@ sample end

TEST(Template, type_traits_is_void_sfinae_f2)
{
    // @@@ sample begin 3:1

    static_assert(!is_void_sfinae_f_v<int>);
    static_assert(!is_void_sfinae_f_v<std::string>);
    static_assert(is_void_sfinae_f_v<void>);
    // @@@ sample end
}
}  // namespace SecondaryImple

// @@@ sample begin 4:0

namespace Inner_ {
template <typename T>
T*& t2ptr();  // 定義は不要
}  // namespace Inner_

template <typename T, typename = void*&>
struct is_void_sfinae_s : std::false_type {
};

// clang-format off
template <typename T>
struct is_void_sfinae_s<
    T,
    // @@@ sample end
    // @@@ sample begin 4:1
    // T != voidの場合、ill-formed
    // T == voidの場合、well-formedでvoid*&生成
    decltype(Inner_::t2ptr<T>() = Inner_::t2ptr<void>())
    // @@@ sample end
    // @@@ sample begin 4:2

    > : std::true_type {
};

template <typename T>
constexpr bool is_void_sfinae_s_v{is_void_sfinae_s<T>::value};
// @@@ sample end
// clang-format on

TEST(Template, type_traits_is_void_sfinae_s)
{
    // @@@ sample begin 4:3

    static_assert(!is_void_sfinae_s_v<int>);
    static_assert(std::is_base_of_v<std::false_type, is_void_sfinae_s<int>>);

    static_assert(!is_void_sfinae_s_v<std::string>);
    static_assert(std::is_base_of_v<std::false_type, is_void_sfinae_s<std::string>>);

    static_assert(is_void_sfinae_s_v<void>);
    static_assert(std::is_base_of_v<std::true_type, is_void_sfinae_s<void>>);
    // @@@ sample end
}

// @@@ sample begin 5:0
template <typename T, typename = void>
struct is_void_ena_s : std::false_type {
};

// clang-format off
template <typename T>
struct is_void_ena_s<
    T,
    // @@@ sample end
    // @@@ sample begin 5:1
    typename std::enable_if_t<is_void_f<T>()>
    // @@@ sample end
    // @@@ sample begin 5:2
    > : std::true_type {
};

template <typename T>
constexpr bool is_void_ena_s_v{is_void_ena_s<T>::value};
// @@@ sample end
// clang-format on

TEST(Template, type_traits_is_void_ena_s)
{
    // @@@ sample begin 5:3

    static_assert(!is_void_ena_s_v<int>);
    static_assert(std::is_base_of_v<std::false_type, is_void_ena_s<int>>);

    static_assert(!is_void_ena_s_v<std::string>);
    static_assert(std::is_base_of_v<std::false_type, is_void_ena_s<std::string>>);

    static_assert(is_void_ena_s_v<void>);
    static_assert(std::is_base_of_v<std::true_type, is_void_ena_s<void>>);
    // @@@ sample end
}

// @@@ sample begin 6:0
template <typename T>
struct is_void_cond_s : std::conditional_t<is_void_f<T>(), std::true_type, std::false_type> {
};

template <typename T>
constexpr bool is_void_cond_s_v{is_void_cond_s<T>::value};
// @@@ sample end

TEST(Template, type_traits_is_void_cond_s)
{
    // @@@ sample begin 6:1

    static_assert(!is_void_cond_s_v<int>);
    static_assert(std::is_base_of_v<std::false_type, is_void_cond_s<int>>);

    static_assert(!is_void_cond_s_v<std::string>);
    static_assert(std::is_base_of_v<std::false_type, is_void_cond_s<std::string>>);

    static_assert(is_void_cond_s_v<void>);
    static_assert(std::is_base_of_v<std::true_type, is_void_cond_s<void>>);
    // @@@ sample end
}
}  // namespace IsVoidXXX
}  // namespace

#include <type_traits>

#include "gtest_wrapper.h"

namespace {
namespace IsSameXXX {

#if 1
// @@@ sample begin 0:0

template <typename T, typename U>
constexpr bool is_same_f_helper(T const*, U const*) noexcept
{
    return false;
}

template <typename T>
constexpr bool is_same_f_helper(T const*, T const*) noexcept
{
    return true;
}

template <typename T, typename U>
constexpr bool is_same_f() noexcept
{
    return is_same_f_helper(static_cast<T*>(nullptr), static_cast<U*>(nullptr));
}

template <typename T, typename U>
constexpr bool is_same_f_v{is_same_f<T, U>()};
// @@@ sample end

#else
// @@@ sample begin 0:1

template <typename T, typename U>
constexpr bool is_same_f()
{
    return true;
}

template <typename T>
constexpr bool is_same_f<T, T>()
{
    return true;
}
// @@@ sample end
#endif

TEST(Template, type_traits_is_same_f)
{
    // @@@ sample begin 0:2

    static_assert(!is_same_f_v<int, void>);
    static_assert(is_same_f_v<int, int>);
    static_assert(!is_same_f_v<int, uint32_t>);
    static_assert(is_same_f_v<std::string, std::basic_string<char>>);
    // @@@ sample end
}

// @@@ sample begin 1:0

template <typename T, typename U>
constexpr bool is_same_v{false};

template <typename T>
constexpr bool is_same_v<T, T>{true};
// @@@ sample end

TEST(Template, type_traits_is_same_v)
{
    // @@@ sample begin 1:1

    static_assert(!is_same_v<int, void>);
    static_assert(is_same_v<int, int>);
    static_assert(!is_same_v<int, uint32_t>);
    static_assert(is_same_v<std::string, std::basic_string<char>>);
    // @@@ sample end
}

// @@@ sample begin 2:0

template <class T, class U>
struct is_same_s {
    static constexpr bool value{false};
};

template <class T>
struct is_same_s<T, T> {
    static constexpr bool value{true};
};

template <typename T, typename U>
constexpr bool is_same_s_v{is_same_s<T, U>::value};
// @@@ sample end

TEST(Template, type_traits_is_same_s)
{
    // @@@ sample begin 2:1

    static_assert(!is_same_s_v<int, void>);
    static_assert(is_same_s_v<int, int>);
    static_assert(!is_same_s_v<int, uint32_t>);
    static_assert(is_same_s_v<std::string, std::basic_string<char>>);
    // @@@ sample end
}

// @@@ sample begin 3:0

namespace Inner_ {
template <typename T, typename U>
constexpr auto is_same_sfinae_f_detector(T const* t, U const* u) noexcept
    // @@@ sample end
    // @@@ sample begin 3:1
    -> decltype(t = u, u = t, bool{})  // T != Uの場合、t = u, u = tはill-formed
                                       // T == Uの場合、well-formedでbool型生成
// @@@ sample end
// @@@ sample begin 3:2
{
    return true;
}

constexpr bool is_same_sfinae_f_detector(...) noexcept { return false; }
}  // namespace Inner_

template <typename T, typename U>
constexpr bool is_same_sfinae_f() noexcept
{
    return Inner_::is_same_sfinae_f_detector(static_cast<T*>(nullptr), static_cast<U*>(nullptr));
}

template <typename T, typename U>
constexpr bool is_same_sfinae_f_v{is_same_sfinae_f<T, U>()};
// @@@ sample end

TEST(Template, type_traits_is_same_sfinae_f)
{
    // @@@ sample begin 3:3

    static_assert(!is_same_sfinae_f_v<int, void>);
    static_assert(is_same_sfinae_f_v<int, int>);
    static_assert(!is_same_sfinae_f_v<int, uint32_t>);
    static_assert(is_same_sfinae_f_v<std::string, std::basic_string<char>>);
    // @@@ sample end
}

// @@@ sample begin 4:0

namespace Inner_ {
template <typename T>
T*& t2ptr();
}

template <typename T, typename U, typename = T*&>
struct is_same_sfinae_s : std::false_type {
};

// clang-format off
template <typename T, typename U>
struct is_same_sfinae_s<
    T, U,

    // T != Uの場合、ill-formed
    // T == Uの場合、well-formedでT*&生成
    decltype(Inner_::t2ptr<T>() = Inner_::t2ptr<U>(), Inner_::t2ptr<U>() = Inner_::t2ptr<T>())
    > : std::true_type {
};
// clang-format on

template <typename T, typename U>
constexpr bool is_same_sfinae_s_v{is_same_sfinae_s<T, U>::value};
// @@@ sample end

TEST(Template, type_traits_is_same_sfinae_s)
{
    // @@@ sample begin 4:1

    static_assert(!is_same_sfinae_s_v<int, void>);
    static_assert(is_same_sfinae_s_v<int, int>);
    static_assert(!is_same_sfinae_s_v<int, uint32_t>);
    static_assert(is_same_sfinae_s_v<std::string, std::basic_string<char>>);
    // @@@ sample end
}

// @@@ sample begin 5:0

template <typename T, template <class...> class TEMPL, typename... ARGS>
struct is_same_templ : is_same_sfinae_s<T, TEMPL<ARGS...>> {
};

template <typename T, template <class...> class TEMPL, typename... ARGS>
constexpr bool is_same_templ_v{is_same_templ<T, TEMPL, ARGS...>::value};
// @@@ sample end

TEST(Template, type_traits_is_same_templ)
{
    // @@@ sample begin 5:1

    static_assert(is_same_s_v<std::string, std::basic_string<char>>);
    static_assert(!is_same_s_v<std::string, std::basic_string<signed char>>);
    // @@@ sample end
    // @@@ sample begin 5:2

    static_assert(is_same_templ_v<std::string, std::basic_string, char>);
    static_assert(!is_same_templ_v<std::string, std::basic_string, signed char>);
    // @@@ sample end
}

// @@@ sample begin 5:3

template <typename T>
using gen_std_string = is_same_templ<std::string, std::basic_string, T>;

template <typename T>
constexpr bool gen_std_string_v{gen_std_string<T>::value};
// @@@ sample end

TEST(Template, type_traits_gen_std_string)
{
    // @@@ sample begin 5:4

    static_assert(gen_std_string_v<char>);
    static_assert(!gen_std_string_v<signed char>);
    // @@@ sample end
}
}  // namespace IsSameXXX
}  // namespace

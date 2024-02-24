#include <list>
#include <type_traits>

#include "gtest_wrapper.h"

#include "nstd_type_traits.h"
#include "suppress_warning.h"

namespace {

namespace ValueTypeNotRecursive {

// リカーシブでないValueType
// @@@ sample begin 0:0

template <typename T, typename = void>
struct ValueType {
    using type = void;
};

template <typename T>
struct ValueType<T, typename std::enable_if_t<std::is_array_v<T>>> {
    using type = typename std::remove_extent_t<T>;
};

template <typename T>
using ValueTypeT = typename ValueType<T>::type;
// @@@ sample end

TEST(Template, value_type_not_recursive)
{
    // @@@ sample begin 0:1

    static_assert(std::is_same_v<int, ValueTypeT<int[1]>>);
    static_assert(std::is_same_v<void, ValueTypeT<int>>);
    static_assert(std::is_same_v<int[2], ValueTypeT<int[1][2]>>);
    // @@@ sample end
}
}  // namespace ValueTypeNotRecursive

namespace ValueTypeNest {

// Nestのサポート
// @@@ sample begin 1:0

template <typename T, typename = void>
struct ValueType {
    using type = void;
    static constexpr size_t Nest{0};
};

template <typename T>
struct ValueType<T, typename std::enable_if_t<std::is_array_v<T>>> {
    using type = typename std::remove_extent_t<T>;

    static constexpr size_t Nest{ValueType<type>::Nest + 1};
};

template <typename T>
using ValueTypeT = typename ValueType<T>::type;
// @@@ sample end

TEST(Template, value_type_recursive)
{
    static_assert(std::is_same_v<int, ValueTypeT<int[1]>>);
    static_assert(std::is_same_v<void, ValueTypeT<int>>);
    static_assert(std::is_same_v<int[2], ValueTypeT<int[1][2]>>);

    // @@@ sample begin 1:1

    static_assert(0 == ValueType<int>::Nest);
    static_assert(1 == ValueType<int[1]>::Nest);
    static_assert(2 == ValueType<int[1][2]>::Nest);
    // @@@ sample end

    static_assert(3 == ValueType<int[1][2][3]>::Nest);
}
}  // namespace ValueTypeNest

namespace ValueTypeN_NS {

// @@@ sample begin 2:0

template <typename T, typename = void>
struct ValueType {
    using type = void;
    static constexpr size_t Nest{0};

    template <size_t N>
    using type_n = typename std::conditional_t<N == 0, T, void>;
};

// @@@ sample end
// @@@ sample begin 2:1

template <typename T, size_t N>
struct ConditionalValueTypeN {
    using type = typename std::conditional_t<
        ValueType<T>::Nest != 0,
        typename ValueType<typename ValueType<T>::type>::template type_n<N - 1>, T>;
};

template <typename T>
struct ConditionalValueTypeN<T, 0> {
    using type = T;
};

template <typename T>
struct ValueType<T, typename std::enable_if_t<std::is_array_v<T>>> {
    using type = typename std::remove_extent_t<T>;

    static constexpr size_t Nest{ValueType<type>::Nest + 1};

    template <size_t N>
    using type_n = typename ConditionalValueTypeN<T, N>::type;
};

template <typename T>
using ValueTypeT = typename ValueType<T>::type;

template <typename T, size_t N>
using ValueTypeT_n = typename ValueType<T>::template type_n<N>;
// @@@ sample end

TEST(Template, type_n)
{
    {
        // @@@ sample begin 2:2

        using T = ValueTypeT_n<int[1][2][3], 3>;
        // @@@ sample end

        static_assert(std::is_same_v<int, T>);
    }
    {
        // @@@ sample begin 2:3

        using T = int[1][2][3];

        static_assert(ValueType<T>::Nest == 3);
        static_assert(std::is_same_v<int[1][2][3], ValueTypeT_n<T, 0>>);
        static_assert(std::is_same_v<int[2][3], ValueTypeT_n<T, 1>>);
        static_assert(std::is_same_v<int[3], ValueTypeT_n<T, 2>>);
        static_assert(std::is_same_v<int, ValueTypeT_n<T, 3>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 4>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 5>>);
        // @@@ sample end
        // @@@ sample begin 2:4

        static_assert(std::is_same_v<int, ValueTypeT_n<T, ValueType<T>::Nest>>);
        // @@@ sample end
    }
    {
        using T = int[1][2];

        static_assert(std::is_same_v<int[1][2], ValueTypeT_n<T, 0>>);
        static_assert(std::is_same_v<int[2], ValueTypeT_n<T, 1>>);
        static_assert(std::is_same_v<int, ValueTypeT_n<T, 2>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 3>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 5>>);
    }
    {
        using T = int[1];

        static_assert(std::is_same_v<int[1], ValueTypeT_n<T, 0>>);
        static_assert(std::is_same_v<int, ValueTypeT_n<T, 1>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 2>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 3>>);
    }
    {
        using T = int;

        static_assert(std::is_same_v<int, ValueTypeT_n<T, 0>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 1>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 2>>);
    }
}
}  // namespace ValueTypeN_NS

namespace ValueTypeN2_NS {

// @@@ sample begin 3:0

template <typename T, typename = void>
struct ValueType {
    using type_direct = void;

    static constexpr size_t Nest{0};

    template <size_t N>
    using type_n = typename std::conditional_t<N == 0, T, void>;

    using type = type_n<Nest>;
};

namespace Inner_ {

template <typename T, size_t N>
struct conditional_value_type_n {
    using type = typename std::conditional_t<
        ValueType<T>::Nest != 0,
        typename ValueType<typename ValueType<T>::type_direct>::template type_n<N - 1>, T>;
};

template <typename T>
struct conditional_value_type_n<T, 0> {
    using type = T;
};
}  // namespace Inner_

template <typename T>
struct ValueType<T, typename std::enable_if_t<std::is_array_v<T>>> {
    using type_direct = typename std::remove_extent_t<T>;

    static constexpr size_t Nest{ValueType<type_direct>::Nest + 1};

    template <size_t N>
    using type_n = typename Inner_::conditional_value_type_n<T, N>::type;

    using type = type_n<Nest>;
};

template <typename T>
using ValueTypeT = typename ValueType<T>::type;

template <typename T, size_t N>
using ValueTypeT_n = typename ValueType<T>::template type_n<N>;
// @@@ sample end

TEST(Template, value_type_n_2)
{
    {
        using T = int[1][2][3];

        static_assert(std::is_same_v<T, ValueTypeT_n<T, 0>>);
        static_assert(std::is_same_v<int[2][3], ValueTypeT_n<T, 1>>);
        static_assert(std::is_same_v<int[3], ValueTypeT_n<T, 2>>);
        static_assert(std::is_same_v<int, ValueTypeT_n<T, 3>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 4>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 5>>);

        static_assert(std::is_same_v<int, ValueTypeT<T>>);
    }
    {
        using T = int[1][2];

        static_assert(std::is_same_v<T, ValueTypeT_n<T, 0>>);
        static_assert(std::is_same_v<int[2], ValueTypeT_n<T, 1>>);
        static_assert(std::is_same_v<int, ValueTypeT_n<T, 2>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 3>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 5>>);

        static_assert(std::is_same_v<int, ValueTypeT<T>>);
    }
    {
        using T = int[1];

        static_assert(std::is_same_v<T, ValueTypeT_n<T, 0>>);
        static_assert(std::is_same_v<int, ValueTypeT_n<T, 1>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 2>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 3>>);

        static_assert(std::is_same_v<int, ValueTypeT<T>>);
    }
    {
        using T = int;

        static_assert(std::is_same_v<T, ValueTypeT_n<T, 0>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 1>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 2>>);

        static_assert(std::is_same_v<T, ValueTypeT<T>>);
    }
}

// @@@ sample begin 4:0

namespace Inner_ {

// Tが配列でなく、且つIsRrangeV<T>が真ならばコンテナと診断する
template <typename T>
constexpr bool is_container_v{Nstd::IsRangeV<T> && !std::is_array_v<T>};
}  // namespace Inner_

template <typename T>
struct ValueType<T, typename std::enable_if_t<Inner_::is_container_v<T>>> {
    using type_direct = typename T::value_type;

    static constexpr size_t Nest{ValueType<type_direct>::Nest + 1};

    template <size_t N>
    using type_n = typename Inner_::conditional_value_type_n<T, N>::type;

    using type = type_n<Nest>;
};

template <typename T>
using ValueTypeT = typename ValueType<T>::type;

template <typename T, size_t N>
using ValueTypeT_n = typename ValueType<T>::template type_n<N>;
// @@@ sample end

TEST(Template, value_type_clone)
{
    {
        // @@@ sample begin 4:1

        using T = std::vector<std::list<int*>[3]>;

        static_assert(std::is_same_v<int*, ValueTypeT<T>>);

        static_assert(std::is_same_v<T, ValueTypeT_n<T, 0>>);
        static_assert(std::is_same_v<std::list<int*>[3], ValueTypeT_n<T, 1>>);
        static_assert(std::is_same_v<std::list<int*>, ValueTypeT_n<T, 2>>);
        static_assert(std::is_same_v<int*, ValueTypeT_n<T, 3>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 4>>);
        // @@@ sample end
    }
    {
        using T = std::vector<int[2][3]>;

        static_assert(std::is_same_v<int, ValueTypeT<T>>);

        static_assert(std::is_same_v<T, ValueTypeT_n<T, 0>>);
        static_assert(std::is_same_v<int[2][3], ValueTypeT_n<T, 1>>);
        static_assert(std::is_same_v<int[3], ValueTypeT_n<T, 2>>);
        static_assert(std::is_same_v<int, ValueTypeT_n<T, 3>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 4>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 5>>);

        static_assert(std::is_same_v<int, ValueTypeT<T>>);
    }
    {
        using T = int[1][2][3];

        static_assert(std::is_same_v<int, ValueTypeT<T>>);

        static_assert(std::is_same_v<T, ValueTypeT_n<T, 0>>);
        static_assert(std::is_same_v<int[2][3], ValueTypeT_n<T, 1>>);
        static_assert(std::is_same_v<int[3], ValueTypeT_n<T, 2>>);
        static_assert(std::is_same_v<int, ValueTypeT_n<T, 3>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 4>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 5>>);

        static_assert(std::is_same_v<int, ValueTypeT<T>>);
    }
    {
        using T = int[1][2];

        static_assert(std::is_same_v<int, ValueTypeT<T>>);

        static_assert(std::is_same_v<T, ValueTypeT_n<T, 0>>);
        static_assert(std::is_same_v<int[2], ValueTypeT_n<T, 1>>);
        static_assert(std::is_same_v<int, ValueTypeT_n<T, 2>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 3>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 5>>);

        static_assert(std::is_same_v<int, ValueTypeT<T>>);
    }
    {
        using T = int[1];

        static_assert(std::is_same_v<int, ValueTypeT<T>>);

        static_assert(std::is_same_v<T, ValueTypeT_n<T, 0>>);
        static_assert(std::is_same_v<int, ValueTypeT_n<T, 1>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 2>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 3>>);

        static_assert(std::is_same_v<int, ValueTypeT<T>>);
    }
    {
        using T = int*;

        static_assert(std::is_same_v<T, ValueTypeT<T>>);

        static_assert(std::is_same_v<T, ValueTypeT_n<T, 0>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 1>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 2>>);
    }
    {
        using T = int;

        static_assert(std::is_same_v<T, ValueTypeT<T>>);

        static_assert(std::is_same_v<T, ValueTypeT_n<T, 0>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 1>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 2>>);
    }
}
}  // namespace ValueTypeN2_NS
}  // namespace

#include <list>

#include "gtest_wrapper.h"

#include "nstd_type_traits.h"
#include "suppress_warning.h"
#include "test_class.h"

namespace Nstd {
namespace {

TEST(Template, is_same_some_of)
{
    // @@@ sample begin 0:0

    static_assert(!Nstd::IsSameSomeOfV<int, int8_t, int16_t, uint16_t>);
    static_assert(Nstd::IsSameSomeOfV<int, int8_t, int16_t, uint16_t, int32_t>);
    static_assert(Nstd::IsSameSomeOfV<int&, int8_t, int16_t, int32_t&, int32_t>);
    static_assert(!Nstd::IsSameSomeOfV<int&, int8_t, int16_t, uint32_t&, int32_t>);
    static_assert(Nstd::IsSameSomeOfV<std::string, int, char*, std::string>);
    static_assert(!Nstd::IsSameSomeOfV<std::string, int, char*>);
    // @@@ sample end
}

TEST(Template, are_convertible)
{
    // @@@ sample begin 1:0

    static_assert(Nstd::AreConvertibleV<int, int8_t, int16_t, int>);
    static_assert(Nstd::AreConvertibleV<int, char, int, int>);
    static_assert(!Nstd::AreConvertibleV<int, char*, int, int>);
    static_assert(Nstd::AreConvertibleV<std::string, std::string, char*, char[3]>);
    static_assert(!Nstd::AreConvertibleV<std::string, std::string, char*, int>);
    // @@@ sample end
}

TEST(Template, are_convertible_without_narrow_conv)
{
    // @@@ sample begin 2:0

    static_assert(Nstd::Inner_::is_convertible_without_narrow_conv_v<int, int>);
    static_assert(Nstd::Inner_::is_convertible_without_narrow_conv_v<int, int16_t>);
    static_assert(!Nstd::Inner_::is_convertible_without_narrow_conv_v<int16_t, int>);
    static_assert(Nstd::Inner_::is_convertible_without_narrow_conv_v<std::string, char*>);
    static_assert(!Nstd::Inner_::is_convertible_without_narrow_conv_v<char*, std::string>);
    // @@@ sample end
    // @@@ sample begin 2:1

    static_assert(Nstd::AreConvertibleWithoutNarrowConvV<int, char, int16_t, uint16_t>);
    static_assert(!Nstd::AreConvertibleWithoutNarrowConvV<int, char, int16_t, uint32_t>);
    static_assert(Nstd::AreConvertibleWithoutNarrowConvV<std::string, char[5], char*>);
    static_assert(Nstd::AreConvertibleWithoutNarrowConvV<double, float>);

    // int8_t -> doubleは縮小型変換
    static_assert(!Nstd::AreConvertibleWithoutNarrowConvV<double, float, int8_t>);
    // @@@ sample end

    static_assert(Nstd::AreConvertibleWithoutNarrowConvV<int, int>);
    static_assert(Nstd::AreConvertibleWithoutNarrowConvV<int, int16_t>);
    static_assert(Nstd::AreConvertibleWithoutNarrowConvV<int, uint16_t>);
    static_assert(Nstd::AreConvertibleWithoutNarrowConvV<int, char>);
    static_assert(Nstd::AreConvertibleWithoutNarrowConvV<int, char, int16_t, uint16_t>);
    static_assert(!Nstd::AreConvertibleWithoutNarrowConvV<int, void>);
    static_assert(!Nstd::AreConvertibleWithoutNarrowConvV<void, void>);
    static_assert(!Nstd::AreConvertibleWithoutNarrowConvV<int, uint32_t>);
    static_assert(Nstd::AreConvertibleWithoutNarrowConvV<double, float>);
    static_assert(!Nstd::AreConvertibleWithoutNarrowConvV<double, int>);
    static_assert(!Nstd::AreConvertibleWithoutNarrowConvV<double, float, int>);
    static_assert(!Nstd::AreConvertibleWithoutNarrowConvV<float, double>);
    static_assert(!Nstd::AreConvertibleWithoutNarrowConvV<float, int>);
    static_assert(Nstd::AreConvertibleWithoutNarrowConvV<std::string, char[5]>);
    static_assert(Nstd::AreConvertibleWithoutNarrowConvV<std::string, char[5], char*>);
}
}  // namespace

namespace Inner_ {
namespace {

TEST(Template, exists_begin)
{
    // @@@ sample begin 3:0

    static_assert(exists_begin_v<std::string>);
    static_assert(!exists_begin_v<int>);
    static_assert(exists_begin_v<int const[3]>);
    static_assert(exists_begin_v<int[3]>);  // 問題が解決

    static_assert(exists_end_v<std::string>);
    static_assert(!exists_end_v<int>);
    static_assert(exists_end_v<int const[3]>);
    static_assert(exists_end_v<int[3]>);
    // @@@ sample end
}
}  // namespace
}  // namespace Inner_

namespace {
TEST(Template, is_range)
{
    // @@@ sample begin 4:0

    static_assert(IsRangeV<std::string>);
    static_assert(!IsRangeV<int>);
    static_assert(IsRangeV<int const[3]>);
    static_assert(IsRangeV<int[3]>);
    // @@@ sample end
}

TEST(Template, exists_put_to)
{
    // @@@ sample begin 5:0

    static_assert(Nstd::ExistsPutToV<bool>);
    static_assert(Nstd::ExistsPutToV<std::string>);
    static_assert(!Nstd::ExistsPutToV<std::vector<int>>);
    static_assert(Nstd::ExistsPutToV<std::vector<int>*>);
    static_assert(Nstd::ExistsPutToV<test_class_exits_put_to>);
    static_assert(!Nstd::ExistsPutToV<test_class_not_exits_put_to>);
    static_assert(Nstd::ExistsPutToV<test_class_not_exits_put_to[3]>);
    // @@@ sample end

    // to suppress warning by clang
    auto oss = std::ostringstream{};
    oss << test_class_exits_put_to{};
}

SUPPRESS_WARN_BEGIN;
SUPPRESS_WARN_CLANG_UNUSED_INTERNAL_DECL;
// @@@ sample begin 5:1

struct T {};

std::ostream& operator<<(std::ostream& os, std::vector<T> const& x)
{
    return os << "T:" << x.size();
}

std::ostream& operator<<(std::ostream&, T const&) = delete;

static_assert(!Nstd::ExistsPutToV<T>);              // std::cout << T{} はできない
static_assert(Nstd::ExistsPutToV<std::vector<T>>);  // std::cout << std::vector<T>{} はできる
static_assert(Nstd::ExistsPutToV<T[3]>);            // std::cout << T[3]{} はできる
// @@@ sample end
SUPPRESS_WARN_END;

TEST(Template, value_type)
{
    {
        using T = std::vector<std::list<int*>[3]>;

        static_assert(std::is_same_v<T, ValueTypeT_n<T, 0>>);
        static_assert(std::is_same_v<std::list<int*>[3], ValueTypeT_n<T, 1>>);
        static_assert(std::is_same_v<std::list<int*>, ValueTypeT_n<T, 2>>);
        static_assert(std::is_same_v<int*, ValueTypeT_n<T, 3>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 4>>);
    }
    {
        using T = std::vector<int[2][3]>;

        static_assert(std::is_same_v<T, ValueTypeT_n<T, 0>>);
        static_assert(std::is_same_v<int[2][3], ValueTypeT_n<T, 1>>);
        static_assert(std::is_same_v<int[3], ValueTypeT_n<T, 2>>);
        static_assert(std::is_same_v<int, ValueTypeT_n<T, 3>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 4>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 5>>);

        static_assert(std::is_same_v<int, ValueTypeT<T>>);
        static_assert(!ValueType<T>::IsBuiltinArray);
    }
    {
        using T = int[1][2][3];

        static_assert(std::is_same_v<T, ValueTypeT_n<T, 0>>);
        static_assert(std::is_same_v<int[2][3], ValueTypeT_n<T, 1>>);
        static_assert(std::is_same_v<int[3], ValueTypeT_n<T, 2>>);
        static_assert(std::is_same_v<int, ValueTypeT_n<T, 3>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 4>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 5>>);

        static_assert(std::is_same_v<int, ValueTypeT<T>>);
        static_assert(ValueType<T>::IsBuiltinArray);
    }
    {
        using T = int[1][2];

        static_assert(std::is_same_v<T, ValueTypeT_n<T, 0>>);
        static_assert(std::is_same_v<int[2], ValueTypeT_n<T, 1>>);
        static_assert(std::is_same_v<int, ValueTypeT_n<T, 2>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 3>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 5>>);

        static_assert(std::is_same_v<int, ValueTypeT<T>>);
        static_assert(ValueType<T>::IsBuiltinArray);
    }
    {
        using T = int[1];

        static_assert(std::is_same_v<T, ValueTypeT_n<T, 0>>);
        static_assert(std::is_same_v<int, ValueTypeT_n<T, 1>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 2>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 3>>);

        static_assert(std::is_same_v<int, ValueTypeT<T>>);
        static_assert(ValueType<T>::IsBuiltinArray);
    }
    {
        using T = int*;

        static_assert(std::is_same_v<T, ValueTypeT_n<T, 0>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 1>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 2>>);

        static_assert(std::is_same_v<T, ValueTypeT<T>>);
        static_assert(!ValueType<T>::IsBuiltinArray);
    }
    {
        using T = int;

        static_assert(std::is_same_v<T, ValueTypeT_n<T, 0>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 1>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 2>>);

        static_assert(std::is_same_v<int, ValueTypeT<T>>);
        static_assert(!ValueType<T>::IsBuiltinArray);
    }
}
}  // namespace
}  // namespace Nstd

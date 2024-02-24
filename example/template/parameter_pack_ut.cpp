#include <type_traits>

#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace SumReturnInt {
namespace {
// @@@ sample begin 0:0

template <typename HEAD>
int sum(HEAD head)
{
    return head;
}

template <typename HEAD, typename... TAIL>
int sum(HEAD head, TAIL... tails)
{
    return head + sum(tails...);
}
// @@@ sample end

TEST(Template, parameter_pack_sum)
{
    // @@@ sample begin 0:1

    ASSERT_EQ(1, sum(1));
    ASSERT_EQ(3, sum(1, 2));
    ASSERT_EQ(6, sum(1, 2, 3));
    ASSERT_FLOAT_EQ(6.0, sum(1, 2.0, 3.0));
    ASSERT_EQ(10, sum(1, 2, 3, 4));

    // @@@ ignore begin
    // @@@ ignore end

    ASSERT_EQ(55, sum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10));

    // @@@ ignore begin
    // @@@ ignore end
    // @@@ sample end

    // @@@ sample begin 0:2

    ASSERT_EQ(2, sum(1, true, false));
    // @@@ sample end
}
}  // namespace
}  // namespace SumReturnInt

namespace SumReturnAuto {
namespace {
// @@@ sample begin 1:0

template <typename HEAD>
auto sum(HEAD head)
{
    // std::is_sameの2パラメータが同一であれば、std::is_same<>::value == true
    static_assert(!std::is_same<HEAD, bool>::value, "arguemnt type must not be bool.");

    return head;
}

template <typename HEAD, typename... TAIL>
auto sum(HEAD head, TAIL... tails)
{
    // std::is_sameの2パラメータが同一であれば、std::is_same<>::value == true
    static_assert(!std::is_same<HEAD, bool>::value, "arguemnt type must not be bool.");

    return head + sum(tails...);
}
// @@@ sample end

TEST(Template, parameter_pack_sum2)
{
    ASSERT_EQ(1, sum(1));
    ASSERT_EQ(3, sum(1, 2));
    ASSERT_EQ(6, sum(1, 2, 3));
    ASSERT_EQ(6.0, sum(1, 2.0, 3.0));
    ASSERT_EQ(10, sum(1, 2, 3, 4));
    ASSERT_EQ(55, sum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10));

    // @@@ sample begin 1:1

    // boolを除く算術型のみ認めるため、下記はコンパイルできない。
    // ASSERT_EQ(2, sum(1, true, false));

    auto i1 = sum(1);
    auto i2 = sum(1, 2);

    static_assert(std::is_same<int, decltype(i1)>::value);  // 1の型はint
    static_assert(std::is_same<int, decltype(i2)>::value);  // 1 + 2の型はint

    auto u1 = sum(1U);
    auto u2 = sum(1U, 2);

    static_assert(std::is_same<unsigned int, decltype(u1)>::value);  // 1Uの型はunsigned int
    static_assert(std::is_same<unsigned int, decltype(u2)>::value);  // 1U + 2の型はunsigned int

    auto f0 = sum(1.0, 1.2);
    static_assert(std::is_same<double, decltype(f0)>::value);

    // ただし、戻り型をautoにしたため、下記も認められるようになった。
    // これに対しての対処は別の関数で行う。
    auto str = sum(std::string{"1"}, std::string{"2"});

    ASSERT_EQ(str, "12");
    static_assert(std::is_same<std::string, decltype(str)>::value);
    // @@@ sample end

    ASSERT_EQ(1, i1);
    ASSERT_EQ(3, i2);
    ASSERT_EQ(1, u1);
    ASSERT_EQ(3, u2);
    ASSERT_EQ(f0, 2.2);
}
}  // namespace
}  // namespace SumReturnAuto

namespace SumFoldExpression {
namespace {

// @@@ sample begin 2:0

template <typename... ARGS>
auto sum(ARGS... args)
{
    return (args + ...);  // 畳みこみ式は()で囲まなければならない。
}
// @@@ sample end

TEST(Template, parameter_pack_fold_exp)
{
    // @@@ sample begin 2:1

    ASSERT_EQ(1, sum(1));
    ASSERT_EQ(3, sum(1, 2));
    ASSERT_EQ(6, sum(1, 2, 3));
    ASSERT_EQ(6.0, sum(1, 2.0, 3.0));
    ASSERT_EQ(10, sum(1, 2, 3, 4));
    ASSERT_EQ(55, sum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10));
    // @@@ sample end
}
}  // namespace
}  // namespace SumFoldExpression

namespace SumProduct {
namespace {
// @@@ sample begin 3:0

template <typename HEAD>
auto product(HEAD head)
{
    static_assert(!std::is_same_v<HEAD, bool>, "arguemnt type must not be bool.");
    static_assert(std::is_arithmetic_v<HEAD>, "arguemnt type must be arithmetic.");

    return head;
}

template <typename HEAD, typename... TAIL>
auto product(HEAD head, TAIL... tails)
{
    static_assert(!std::is_same_v<HEAD, bool>, "arguemnt type must not be bool.");
    static_assert(std::is_arithmetic_v<HEAD>, "arguemnt type must be arithmetic.");

    return head * product(tails...);
}
// @@@ sample end

TEST(Template, parameter_pack_product)
{
#if 0
    // @@@ sample begin 3:1

    ASSERT_EQ(1, product(100, 0.1, 0.1));
    // @@@ sample end
#else
    // @@@ sample begin 3:2

    ASSERT_EQ(1, product(0.1, 0.1, 100));

    // @@@ sample end
#endif

    {
        int const a = product(100, 0.1, 0.1);
        int const b = product(0.1, 0.1, 100);
        ASSERT_EQ(a, b);
    }
}
}  // namespace
}  // namespace SumProduct

namespace SumProduct2 {
namespace {
// @@@ sample begin 4:0

template <typename HEAD>
auto product(HEAD head)
{
    static_assert(!std::is_same_v<HEAD, bool>, "arguemnt type must not be bool.");
    static_assert(std::is_arithmetic_v<HEAD>, "arguemnt type must be arithmetic.");

    return head;
}

template <typename HEAD, typename HEAD2, typename... TAIL>
auto product(HEAD head, HEAD2 head2, TAIL... tails)
{
    static_assert(!std::is_same_v<HEAD, bool>, "arguemnt type must not be bool.");
    static_assert(std::is_arithmetic_v<HEAD>, "arguemnt type must be arithmetic.");

    return product(head * head2, tails...);
}
// @@@ sample end

TEST(Template, parameter_pack_product2)
{
#if 1
    // @@@ sample begin 4:1

    ASSERT_EQ(1, product(100, 0.1, 0.1));
    // @@@ sample end
#else
    // @@@ sample begin 4:2

    ASSERT_EQ(1, product(0.1, 0.1, 100));

    // @@@ sample end
#endif
}
}  // namespace
}  // namespace SumProduct2

namespace SumAny {

template <typename I>
auto operator+(I i, std::string const& str) ->
    typename std::enable_if_t<std::is_integral_v<I>, std::string>

{
    return std::to_string(i) + str;
}

template <typename I>
auto operator+(std::string const& str, I i) ->
    typename std::enable_if_t<std::is_integral_v<I>, std::string>

{
    return str + std::to_string(i);
}

template <typename HEAD>
auto sum(HEAD const& head)
{
    return head;
}

template <typename HEAD, typename HEAD2, typename... TAIL>
auto sum(HEAD const& head, HEAD2 const& head2, TAIL... tails)
{
    auto new_head = head + head2;
    return sum(new_head, tails...);
}

TEST(Template, parameter_pack)
{
    ASSERT_EQ(1, sum(1));
    ASSERT_EQ(3, sum(1, 2));

    ASSERT_EQ("1", sum(std::string{"1"}));

    auto a = sum(std::string{"1"}, "2", "3");
    ASSERT_EQ("123", a);

    auto b = sum(std::string{"1"}, 2, 3);
    ASSERT_EQ("123", b);
}
}  // namespace SumAny

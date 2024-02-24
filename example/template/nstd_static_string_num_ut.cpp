#include "gtest_wrapper.h"

#include "nstd_static_string_num.h"

namespace Nstd {
namespace Inner_ {
namespace {

TEST(Template, static_string_num_num_of_digits)
{
    static_assert(5 == num_of_digits(12345));
    static_assert(4 == num_of_digits(1234));
    static_assert(3 == num_of_digits(123));
    static_assert(2 == num_of_digits(12));
    static_assert(1 == num_of_digits(1));
    static_assert(0 == num_of_digits(0));
}

TEST(Template, static_string_num_ten_to_nth_power)
{
    static_assert(1 == ten_to_nth_power(0));
    static_assert(10 == ten_to_nth_power(1));
    static_assert(100 == ten_to_nth_power(2));
    static_assert(1000 == ten_to_nth_power(3));
    static_assert(10000 == ten_to_nth_power(4));
    static_assert(100000 == ten_to_nth_power(5));
}

TEST(Template, static_string_num_reverse_num)
{
    static_assert(321 == reverse_num(123));
    static_assert(54321 == reverse_num(12345));
    static_assert(987654321 == reverse_num(123456789));
}

TEST(Template, static_string_num_digit_to_char)
{
    static_assert('5' == digit_to_char(12345, 0));
    static_assert('4' == digit_to_char(12345, 1));
    static_assert('3' == digit_to_char(12345, 2));
    static_assert('2' == digit_to_char(12345, 3));
    static_assert('1' == digit_to_char(12345, 4));
}

TEST(Template, static_string_num_make_static_string)
{
    // @@@ sample begin 0:0

    constexpr auto ns
        = make_static_string<__LINE__>(std::make_index_sequence<Inner_::num_of_digits(__LINE__)>());
    auto line_num = __LINE__ - 1;

    ASSERT_EQ(std::to_string(line_num), ns.String());
    // @@@ sample end
}

TEST(Template, static_string_num)
{
    {
        constexpr auto ns = Int2StaticString<12345>();

        ASSERT_EQ(std::string{"12345"}, ns.String());
        static_assert(std::is_same_v<char const(&)[6], decltype(ns.String())>);
    }
    {
        // @@@ sample begin 1:0

        constexpr auto ns       = Int2StaticString<__LINE__>();
        auto           line_num = __LINE__ - 1;

        ASSERT_EQ(std::to_string(line_num), ns.String());
        // @@@ sample end
    }
}
}  // namespace
}  // namespace Inner_
}  // namespace Nstd

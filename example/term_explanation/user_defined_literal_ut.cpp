#include "gtest_wrapper.h"

namespace {
// @@@ sample begin 0:0

constexpr int32_t one_km = 1000;

// ユーザ定義リテラル演算子の定義
constexpr int32_t operator""_kilo_meter(unsigned long long num_by_mk) { return num_by_mk * one_km; }
constexpr int32_t operator""_meter(unsigned long long num_by_m) { return num_by_m; }
// @@@ sample end

TEST(UDL, user_defined_literal)
{
    // @@@ sample begin 0:1

    int32_t km = 3_kilo_meter;  // ユーザ定義リテラル演算子の利用
    int32_t m  = 3000_meter;    // ユーザ定義リテラル演算子の利用

    ASSERT_EQ(m, km);
    // @@@ sample end
}

TEST(UDL, std_string)
{
    // @@@ sample begin 1:0

    using namespace std::literals::string_literals;

    auto a = "str"s;  // aはstd::string
    auto b = "str";   // bはconst char*

    static_assert(std::is_same_v<decltype(a), std::string>);
    ASSERT_EQ(std::string{"str"}, a);

    static_assert(std::is_same_v<decltype(b), char const*>);
    ASSERT_STREQ("str", b);
    // @@@ sample end
}
}  // namespace

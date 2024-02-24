#include <iostream>
#include <list>

#include "gtest_wrapper.h"

#include "nstd_put_to.h"
#include "nstd_safe_index.h"
#include "suppress_warning.h"
#include "test_class.h"

// for test
namespace TestNS {
class X2 {
public:
    X2(int i) noexcept : i_{i} {}
    operator int() const noexcept { return i_; }

private:
    int const i_;
};

std::ostream& operator<<(std::ostream& os, X2 const& x2) { return os << std::to_string(x2); }
}  // namespace TestNS

namespace Nstd {
namespace Inner_ {
namespace {

TEST(Template, enable_put_to_if_v)
{
    // @@@ sample begin 0:0

    static_assert(enable_range_put_to_v<int[3]>);                         // Nstd::operator<<
    static_assert(!enable_range_put_to_v<char[3]>);                       // std::operator<<
    static_assert(!enable_range_put_to_v<int>);                           // std::operator<<
    static_assert(enable_range_put_to_v<std::vector<int>>);               // Nstd::operator<<
    static_assert(enable_range_put_to_v<std::vector<std::vector<int>>>);  // Nstd::operator<<
    static_assert(!enable_range_put_to_v<std::string>);                   // std::operator<<
    static_assert(enable_range_put_to_v<std::vector<std::string>>);       // Nstd::operator<<

    static_assert(!enable_range_put_to_v<test_class_not_exits_put_to>);  // operator<<無し
    static_assert(!enable_range_put_to_v<test_class_exits_put_to>);  // ユーザ定義operator<<
    static_assert(
        !enable_range_put_to_v<std::vector<test_class_not_exits_put_to>>);       // operator<<無し
    static_assert(enable_range_put_to_v<std::vector<test_class_exits_put_to>>);  // Nstd::operator<<
    static_assert(
        !enable_range_put_to_v<std::list<test_class_not_exits_put_to>>);       // operator<<無し
    static_assert(enable_range_put_to_v<std::list<test_class_exits_put_to>>);  // Nstd::operator<<
    // @@@ sample end

    static_assert(enable_range_put_to_v<std::list<TestNS::X2>>);
}
}  // namespace
}  // namespace Inner_
}  // namespace Nstd

namespace {

TEST(Template, builtin_array_put_to)
{
    // @@@ sample begin 1:0

    using namespace Nstd;
    {
        char str[] = "abcdef";
        auto oss   = std::ostringstream{};

        oss << str;  // std::operator<<
        ASSERT_EQ(str, oss.str());
    }
    {
        char str[2][4] = {"abc", "def"};
        auto oss       = std::ostringstream{};

        oss << str;  // Nstd::operator<<
        ASSERT_EQ("abc | def", oss.str());
    }
    {
        test_class_exits_put_to p1[3]{1, 2, 3};
        auto                    oss = std::ostringstream{};

        oss << p1;  // Nstd::operator<<
        ASSERT_EQ("1, 2, 3", oss.str());
    }
    {
        char const* str[] = {"abc", "def", "ghi"};
        auto        oss   = std::ostringstream{};

        oss << str;  // Nstd::operator<<
        ASSERT_EQ("abc, def, ghi", oss.str());
    }
    {
        int  v[2][3][2]{{{0, 1}, {2, 3}, {4, 5}}, {{6, 7}, {8, 9}, {10, 11}}};
        auto oss = std::ostringstream{};

        oss << v;
        ASSERT_EQ("0, 1 | 2, 3 | 4, 5 # 6, 7 | 8, 9 | 10, 11", oss.str());
    }
    // @@@ sample end
    {
        int  v[4][2]{{1, 2}, {3, 4}, {5, 6}, {7, 8}};
        auto oss = std::ostringstream{};

        oss << v;  // Nstd::operator<<
        ASSERT_EQ("1, 2 | 3, 4 | 5, 6 | 7, 8", oss.str());
    }
    {
        TestNS::X2 x2[2][3]{{0, 1, 2}, {3, 4, 5}};
        auto       oss = std::ostringstream{};

        oss << x2;
        ASSERT_EQ("0, 1, 2 | 3, 4, 5", oss.str());
    }
}

TEST(Template, container_put_to)
{
    // @@@ sample begin 2:0

    using namespace Nstd;
    {
        auto v   = std::vector<int>{1, 2, 3};
        auto oss = std::ostringstream{};

        oss << v;
        ASSERT_EQ("1, 2, 3", oss.str());
    }
    {
        auto p1  = std::list<test_class_exits_put_to>{1, 2, 3, 4};
        auto oss = std::ostringstream{};

        oss << p1;
        ASSERT_EQ("1, 2, 3, 4", oss.str());
    }
    {
        std::vector<int> v[2]{{1, 2}, {3, 4, 5}};  // std::vectorの配列
        auto             oss = std::ostringstream{};

        oss << v;
        ASSERT_EQ("1, 2 | 3, 4, 5", oss.str());
    }
    // @@@ sample end
    {
        auto v   = std::vector<std::string>{"1", "2", "3"};
        auto oss = std::ostringstream{};

        oss << v;
        ASSERT_EQ("1 | 2 | 3", oss.str());
    }
    {
        auto v   = std::vector<std::vector<int>>{{1, 2}, {3, 4}, {5, 6}, {7, 8}};
        auto oss = std::ostringstream{};

        oss << v;
        ASSERT_EQ("1, 2 | 3, 4 | 5, 6 | 7, 8", oss.str());
    }
}

TEST(Template, nstd_container_put_to)
{
    {
        auto sv  = Nstd::SafeVector<int>{1, 2, 3};
        auto oss = std::ostringstream{};

        oss << sv;
        ASSERT_EQ("1, 2, 3", oss.str());
    }
    // @@@ sample begin 3:0

    {
        auto sal_s = Nstd::SafeArray<std::string, 4>{"1", "2", "3"};
        auto oss   = std::ostringstream{};

        oss << sal_s;
        ASSERT_EQ("1 | 2 | 3 | ", oss.str());
    }
    {
        auto sv
            = Nstd::SafeVector<Nstd::SafeArray<Nstd::SafeString, 2>>{{"ab", "cd"}, {"ef", "gh"}};
        auto oss = std::ostringstream{};

        oss << sv;
        ASSERT_EQ("ab | cd # ef | gh", oss.str());
    }
    // @@@ sample end
}
}  // namespace

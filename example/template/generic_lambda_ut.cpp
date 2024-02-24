#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

#include "gtest_wrapper.h"

#include "nstd_put_to.h"
#include "nstd_safe_index.h"
#include "suppress_warning.h"

namespace {
// @@@ sample begin 0:0

struct XYZ {
    XYZ(int ax, int ay, int az) noexcept : x{ax}, y{ay}, z{az} {}
    int x;
    int y;
    int z;
};

std::ostream& operator<<(std::ostream& os, XYZ const& xyz)
{
    return os << std::to_string(xyz.x) + "/" + std::to_string(xyz.y) + "/" + std::to_string(xyz.z);
}
// @@@ sample end

TEST(Template, generic_lambda_sort)
{
    {
        // @@@ sample begin 0:1

        auto v   = Nstd::SafeVector<XYZ>{{9, 8, 7}, {6, 5, 4}, {3, 2, 1}, {0, 1, 2}};
        auto oss = std::ostringstream{};

        oss << v;
        ASSERT_EQ("9/8/7, 6/5/4, 3/2/1, 0/1/2", oss.str());
        // @@@ sample end
    }
    {
        // @@@ sample begin 0:2

        auto v = Nstd::SafeVector<XYZ>{{9, 8, 7}, {6, 5, 4}, {3, 2, 1}, {0, 1, 2}};

        // C++11 lambda                  型の明示が必要
        //                               ↓               ↓
        std::sort(v.begin(), v.end(), [](XYZ const& lhs, XYZ const& rhs) noexcept {
            return std::tie(lhs.x, lhs.y, lhs.z) < std::tie(rhs.x, rhs.y, rhs.z);
        });
        auto oss = std::ostringstream{};

        oss << v;
        ASSERT_EQ("0/1/2, 3/2/1, 6/5/4, 9/8/7", oss.str());
        // @@@ sample end
    }
    {
        // @@@ sample begin 0:3

        auto v = Nstd::SafeVector<XYZ>{{9, 8, 7}, {6, 5, 4}, {3, 2, 1}, {0, 1, 2}};

        // C++14 generic lambda          型の明示が不要
        //                               ↓                ↓
        std::sort(v.begin(), v.end(), [](auto const& lhs, auto const& rhs) noexcept {
            return std::tie(lhs.x, lhs.y, lhs.z) < std::tie(rhs.x, rhs.y, rhs.z);
        });
        auto oss = std::ostringstream{};

        oss << v;
        ASSERT_EQ("0/1/2, 3/2/1, 6/5/4, 9/8/7", oss.str());
        // @@@ sample end
    }
}
// @@@ sample begin 1:0

template <typename PUTTO>
void f(PUTTO&& p)
{
    p(1);       // ラムダの引数elemの型はint
    p(2.71);    // ラムダの引数elemは型はdouble
    p("hehe");  // ラムダの引数elemは型はchar [5]
}

TEST(Template, generic_lambda)
{
    auto oss = std::ostringstream{};

    f([&oss](auto const& elem) { oss << elem << std::endl; });

    ASSERT_EQ("1\n2.71\nhehe\n", oss.str());
}
// @@@ sample end
// @@@ sample begin 1:1

class Closure {
public:
    Closure(std::ostream& os) : os_{os} {}

    template <typename T>
    void operator()(T&& t)
    {
        os_ << t << std::endl;
    }

private:
    std::ostream& os_;
};

TEST(Template, generic_lambda_like)
{
    auto oss = std::ostringstream{};

    auto closure = Closure{oss};
    f(closure);

    ASSERT_EQ("1\n2.71\nhehe\n", oss.str());
}
// @@@ sample end
}  // namespace

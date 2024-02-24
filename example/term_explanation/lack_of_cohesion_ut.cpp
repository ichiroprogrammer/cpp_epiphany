#include <cmath>
#include <utility>

#include "gtest_wrapper.h"

namespace Cohesion {
// @@@ sample begin 0:0

class ABC {
public:
    explicit ABC(int32_t a, int32_t b, int32_t c) noexcept : a_{a}, b_{b}, c_{c} {}

    int32_t GetA() const noexcept { return a_; }
    int32_t GetB() const noexcept { return b_; }
    int32_t GetC() const noexcept { return c_; }
    void    SetA(int32_t a) noexcept { a_ = a; }
    void    SetB(int32_t b) noexcept { b_ = b; }
    void    SetC(int32_t c) noexcept { c_ = c; }

private:
    int32_t a_;
    int32_t b_;
    int32_t c_;
};
// @@@ sample end
// @@@ sample begin 0:1

class QuadraticEquation {  // 2次方程式
public:
    explicit QuadraticEquation(int32_t a, int32_t b, int32_t c) noexcept : a_{a}, b_{b}, c_{c} {}

    void Set(int32_t a, int32_t b, int32_t c) noexcept
    {
        a_ = a;
        b_ = b;
        c_ = c;
    }

    int32_t Discriminant() const noexcept  // 判定式
    {
        return b_ * b_ - 4 * a_ * c_;
    }

    bool HasRealNumberSolution() const noexcept { return 0 <= Discriminant(); }

    std::pair<int32_t, int32_t> Solution() const;

private:
    int32_t a_;
    int32_t b_;
    int32_t c_;
};

std::pair<int32_t, int32_t> QuadraticEquation::Solution() const
{
    if (!HasRealNumberSolution()) {
        throw std::invalid_argument{"solution is an imaginary number"};
    }

    auto a0 = static_cast<int32_t>((-b_ - std::sqrt(Discriminant())) / 2);
    auto a1 = static_cast<int32_t>((-b_ + std::sqrt(Discriminant())) / 2);

    return {a0, a1};
}
// @@@ sample end

TEST(Template, cohesion)
{
    {
        auto q = QuadraticEquation{1, -2, 1};

        ASSERT_TRUE(q.HasRealNumberSolution());
        ASSERT_EQ(0, q.Discriminant());

        ASSERT_EQ((std::pair{1, 1}), q.Solution());  // C++14 style

        auto [a, b] = q.Solution();  // C++17 style
        ASSERT_EQ(1, a);
        ASSERT_EQ(1, b);
    }
    {
        auto q = QuadraticEquation{1, -5, 6};

        ASSERT_TRUE(q.HasRealNumberSolution());
        ASSERT_EQ(1, q.Discriminant());
        ASSERT_EQ((std::pair{2, 3}), q.Solution());  // C++14 style

        auto [a, b] = q.Solution();  // C++17 style
        ASSERT_EQ(2, a);
        ASSERT_EQ(3, b);
    }
    {
        auto q = QuadraticEquation{1, 1, 1};

        ASSERT_FALSE(q.HasRealNumberSolution());
        ASSERT_GT(0, q.Discriminant());
        ASSERT_THROW(q.Solution(), std::invalid_argument);
    }
}
}  // namespace Cohesion

#include "gtest_wrapper.h"

namespace {
// @@@ sample begin 0:0

template <typename PUTTO>
void f(PUTTO&& p)
{
    p(1);
    p(2.71);
    p("str");
}

TEST(Template, generic_lambda)
{
    std::ostringstream oss;

    f([&oss](auto const& elem) { oss << elem << std::endl; });

    ASSERT_EQ("1\n2.71\nstr\n", oss.str());
}
// @@@ sample end
// @@@ sample begin 0:1

class Closure {
public:
    Closure(std::ostream& os) : os_(os) {}

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
    std::ostringstream oss;

    Closure closure(oss);
    f(closure);

    ASSERT_EQ("1\n2.71\nstr\n", oss.str());
}
// @@@ sample end
}  // namespace

#include <cstdint>
#include <memory>

#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace {
TEST(AmbiguousOwnership, ambiguous)
{
    // @@@ sample begin 0:0

    class A {
        // 何らかの宣言
    };

    class X {
    public:
        explicit X(A* a) : a_{a} {}
        A* GetA() { return a_; }

    private:
        A* a_;
    };

    auto* a = new A;
    auto  x = X{a};
    // aがxに排他所有されているのか否かの判断は難しい

    auto x0 = X{new A};
    auto x1 = X{x0.GetA()};
    // x0生成時にnewされたオブジェクトがx0とx1に共有所有されているのか否かの判断は難しい
    // @@@ sample end

    IGNORE_UNUSED_VAR(x, x0, x1);
    delete a;
    delete x0.GetA();
}
}  // namespace

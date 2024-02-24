#include "gtest_wrapper.h"

namespace {
TEST(IncompleteType, incomplete_type)
{
    // @@@ sample begin 0:0

    class A;  // Aの前方宣言
              // これ以降、Aは不完全型となる

    // auto a = sizeof(A);  Aが不完全型であるため、コンパイルエラー

    class A {  // この宣言により、この行以降はAは完全型になる
    public:
        // 何らかの宣言
    };

    auto a = sizeof(A);  // Aが完全型であるため、コンパイル可能
    // @@@ sample end
    ASSERT_EQ(1, a);
}
}  // namespace

#include "gtest_wrapper.h"

namespace ReferenceCollapsing {

TEST(Template, reference_collapsing)
{
    // @@@ sample begin 0:0

    int i;

    using IR  = int&;
    using IRR = IR&;  // IRRはint& &となり、int&に変換される

    IR  ir  = i;
    IRR irr = ir;

    static_assert(std::is_same_v<int&, decltype(ir)>);   // lvalueリファレンス
    static_assert(std::is_same_v<int&, decltype(irr)>);  // lvalueリファレンス
    // @@@ sample end

    decltype(ir)& irr2 = ir;  // irrはlvalueリファレンスのリファレンスに見えるが。

    static_assert(std::is_same_v<int&, decltype(irr2)>);  // lvalueリファレンス
}

// @@@ sample begin 0:1

template <typename T>
struct Ref {
    T&  t;
    T&& u;
};
// @@@ sample end

TEST(Template, reference_collapsing2)
{
    // clang-format off
    // @@@ sample begin 0:2

    static_assert(std::is_same_v<int&, decltype(Ref<int>::t)>);    // lvalueリファレンス
    static_assert(std::is_same_v<int&&, decltype(Ref<int>::u)>);   // rvalueリファレンス

    static_assert(std::is_same_v<int&, decltype(Ref<int&>::t)>);   // lvalueリファレンス
    static_assert(std::is_same_v<int&, decltype(Ref<int&>::u)>);   // lvalueリファレンス

    static_assert(std::is_same_v<int&, decltype(Ref<int&&>::t)>);  // lvalueリファレンス
    static_assert(std::is_same_v<int&&, decltype(Ref<int&&>::u)>); // rvalueリファレンス
    // @@@ sample end
    // clang-format on
}

// @@@ sample begin 0:3

template <typename T>
struct AddRef {
    using type = T&;
};
// @@@ sample end
// @@@ sample begin 0:4

template <typename T>
struct AddRef<T&> {
    using type = T&;
};
// @@@ sample end

TEST(Template, reference_collapsing3)
{
    // @@@ sample begin 0:5

    static_assert(std::is_same_v<int&, AddRef<int&>::type>);
    // @@@ sample end
}
}  // namespace ReferenceCollapsing

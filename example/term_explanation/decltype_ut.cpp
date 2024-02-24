#include "gtest_wrapper.h"

namespace DecltypeSample {
namespace {
TEST(Decltype, aut_decltype)
{
    // @@@ sample begin 0:0

    int32_t  x{3};
    int32_t& r{x};

    auto           a = r;  // aの型はint32_t
    decltype(r)    b = r;  // bの型はint32_t&
    decltype(auto) c = r;  // cの型はint32_t&   C++14からサポート
                           // decltype(auto)は、decltypeに右辺の式を与えるための構文

    // std::is_sameはオペランドの型が同じか否かを返すメタ関数
    static_assert(std::is_same_v<decltype(a), int>);
    static_assert(std::is_same_v<decltype(b), int&>);
    static_assert(std::is_same_v<decltype(c), int&>);
    // @@@ sample end

    ASSERT_EQ(3, a);
}
}  // namespace
// @@@ sample begin 0:1

//  本来ならばA::dataは、
//      * A::Aでメモリ割り当て
//      * A::~Aでメモリ解放
//  すべきだが、何らかの理由でそれが出来ないとする
struct A {
    size_t   len;
    uint8_t* data;
};

void do_something(size_t len)
{
    auto deallocate = [](A* p) {
        delete[](p->data);
        delete p;
    };

    auto a_ptr = std::unique_ptr<A, decltype(deallocate)>{new A, deallocate};

    a_ptr->len  = len;
    a_ptr->data = new uint8_t[10];

    // @@@ ignore begin
    // @@@ ignore end
    // do something for a_ptr
    // @@@ ignore begin
    // @@@ ignore end

    // a_ptrによるメモリの自動解放
}
// @@@ sample end

namespace {
TEST(Decltype, decltype_unique_ptr) { do_something(10); }
}  // namespace
}  // namespace DecltypeSample

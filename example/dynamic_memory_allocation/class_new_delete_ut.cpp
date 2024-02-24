#include <memory>

#include "gtest_wrapper.h"

#include "mpool_fixed.h"
#include "mpool_variable.h"
#include "op_new.h"
#include "op_new_deleted.h"
#include "utils.h"

namespace MPF_NotDerived {

// @@@ sample begin 0:0

struct A {
    A() noexcept : name0{"A"} {}
    char const* name0;
    // @@@ sample end
    // @@@ sample begin 0:1

    [[nodiscard]] static void* operator new(size_t size);
    static void                operator delete(void* mem) noexcept;
    static void                operator delete(void* mem, std::size_t size) noexcept;

    [[nodiscard]] static void* operator new[](size_t size)                             = delete;
    static void                operator delete[](void* mem) noexcept                   = delete;
    static void                operator delete[](void* mem, std::size_t size) noexcept = delete;
    // @@@ sample end
    // @@@ sample begin 0:2
};

MPoolFixed<sizeof(A), 3> mpf_A;

void* A::operator new(size_t size) { return mpf_A.Alloc(size); }
void A::operator delete(void* mem) noexcept { mpf_A.Free(mem); }
void A::operator delete(void* mem, std::size_t) noexcept { mpf_A.Free(mem); }
// @@@ sample end

namespace {
TEST(NewDelete, class_new_delete_fixed_not_derived)
{
    // @@@ sample begin 1:0

    ASSERT_EQ(3, mpf_A.GetCount());

    {
        auto a = std::make_unique<A>();
        ASSERT_STREQ("A", a->name0);
        ASSERT_EQ(2, mpf_A.GetCount());
    }
    ASSERT_EQ(3, mpf_A.GetCount());

    {
        auto a = std::make_unique<A>();
        ASSERT_STREQ("A", a->name0);
        ASSERT_EQ(2, mpf_A.GetCount());

        auto b = std::make_unique<A>();
        ASSERT_STREQ("A", b->name0);
        ASSERT_EQ(1, mpf_A.GetCount());

        auto c = std::make_unique<A>();
        ASSERT_STREQ("A", c->name0);
        ASSERT_EQ(0, mpf_A.GetCount());

        ASSERT_THROW(std::make_unique<A>(), MPoolBadAlloc);
    }
    ASSERT_EQ(3, mpf_A.GetCount());
    // @@@ sample end
}
}  // namespace
}  // namespace MPF_NotDerived

// 編集の都合上、以下はグローバル名前空間

// @@@ sample begin 2:0

struct A : public OpNew<A> {
    A() noexcept : name0{"A"} {}
    char const* name0;
};

struct B : A {
    B() noexcept : name1{"B"} {}
    char const* name1;
};

struct C : A {
    C() noexcept : name1{"C"} {}
    char const* name1;
};

struct D : C {
    D() noexcept : name2{"D"} {}
    char const* name2;
};

MPoolFixed<MaxSizeof<A, B, C, D>(), 10> mpf_ABCD;

template <>
MPool& OpNew<A>::mpool_ = mpf_ABCD;
// @@@ sample end

namespace {
TEST(NewDelete, class_new_delete_fixed_derived)
{
    ASSERT_EQ(10, mpf_ABCD.GetCount());
    {
        auto b = std::make_unique<B>();
        ASSERT_STREQ("A", b->name0);
        ASSERT_STREQ("B", b->name1);
        ASSERT_EQ(9, mpf_ABCD.GetCount());
    }
    ASSERT_EQ(10, mpf_ABCD.GetCount());

    {
        auto b = std::make_unique<B>();
        ASSERT_STREQ("A", b->name0);
        ASSERT_STREQ("B", b->name1);
        ASSERT_EQ(9, mpf_ABCD.GetCount());

        auto c = std::make_unique<C>();
        auto d = std::make_unique<D>();
        ASSERT_STREQ("A", c->name0);
        ASSERT_STREQ("C", c->name1);
        ASSERT_STREQ("A", d->name0);
        ASSERT_STREQ("C", d->name1);
        ASSERT_STREQ("D", d->name2);
        ASSERT_EQ(7, mpf_ABCD.GetCount());
    }
    ASSERT_EQ(10, mpf_ABCD.GetCount());
}

// @@@ sample begin 2:1

struct Large : A {
    uint8_t buff[1024];  // mpf_ABCDのメモリブロックのサイズを超える
};

TEST(NewDelete, class_new_delete_fixed_derived_large)
{
    ASSERT_EQ(10, mpf_ABCD.GetCount());
    ASSERT_THROW(auto large = std::make_unique<Large>(), MPoolBadAlloc);  // サイズが大きすぎる
}
// @@@ sample end
}  // namespace

namespace Usage_OpNewDeleted {
// @@@ sample begin 3:0

class DeletedNew : OpNewDeleted {  // プライベート継承
};

class DelivedDeletedNew : DeletedNew {  // プライベート継承
};

// DeletedNew* ptr0 { new DeletedNew };                // OpNewDeletedの効果でコンパイルエラー
// DelivedDeletedNew* ptr1 { new DelivedDeletedNew };  // 同上
// @@@ sample end
}  // namespace Usage_OpNewDeleted

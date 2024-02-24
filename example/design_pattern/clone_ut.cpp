#include <iostream>
#include <memory>

#include "gtest_wrapper.h"

namespace {
// スライシングを起こしてしまうクラス
// @@@ sample begin 0:0

class BaseSlicing {
public:
    // @@@ ignore begin
    BaseSlicing()          = default;
    virtual ~BaseSlicing() = default;
    // @@@ ignore end
    virtual char const* Name() const noexcept { return "BaseSlicing"; }
};

class DerivedSlicing final : public BaseSlicing {
public:
    // @@@ ignore begin
    DerivedSlicing()          = default;
    virtual ~DerivedSlicing() = default;
    // @@@ ignore end
    virtual char const* Name() const noexcept override { return "DerivedSlicing"; }
};

TEST(Clone, object_slicing)
{
    auto b = BaseSlicing{};
    auto d = DerivedSlicing{};

    BaseSlicing* b_ptr   = &b;
    BaseSlicing* b_ptr_d = &d;

    ASSERT_STREQ("BaseSlicing", b_ptr->Name());
    ASSERT_STREQ("DerivedSlicing", b_ptr_d->Name());

    *b_ptr = *b_ptr_d;  // コピーしたつもりだがスライシングにより、*b_ptrは、
                        // DerivedSlicingのインスタンスではなく、BaseSlicingのインスタンス

#if 0
    ASSERT_STREQ("DerivedSlicing", b_ptr->Name());
#else
    ASSERT_STREQ("BaseSlicing", b_ptr->Name());  // "DerivedSlicing"が返るはずだが、
                                                 // スライシングにより"BaseSlicing"が返る
#endif
}
// @@@ sample end
// @@@ sample begin 0:1

// スライシングを起こさないようにコピー演算子の代わりにClone()を実装。
class BaseNoSlicing {
public:
    // @@@ ignore begin
    BaseNoSlicing()          = default;
    virtual ~BaseNoSlicing() = default;
    // @@@ ignore end
    virtual char const* Name() const noexcept { return "BaseNoSlicing"; }

    virtual std::unique_ptr<BaseNoSlicing> Clone() { return std::make_unique<BaseNoSlicing>(); }

    BaseNoSlicing(BaseNoSlicing const&)            = delete;  // copy生成の禁止
    BaseNoSlicing& operator=(BaseNoSlicing const&) = delete;  // copy代入の禁止
};

class DerivedNoSlicing final : public BaseNoSlicing {
public:
    // @@@ ignore begin
    DerivedNoSlicing()          = default;
    virtual ~DerivedNoSlicing() = default;
    // @@@ ignore end
    virtual char const* Name() const noexcept override { return "DerivedNoSlicing"; }

    std::unique_ptr<DerivedNoSlicing> CloneOwn() { return std::make_unique<DerivedNoSlicing>(); }

    // DerivedNoSlicingはBaseNoSlicingの派生クラスであるため、
    // std::unique_ptr<DerivedNoSlicing>オブジェクトから
    // std::unique_ptr<BaseNoSlicing>オブジェクトへのmove代入可能
    virtual std::unique_ptr<BaseNoSlicing> Clone() override { return CloneOwn(); }
};

TEST(Clone, object_slicing_avoidance)
{
    auto b = BaseNoSlicing{};
    auto d = DerivedNoSlicing{};

    BaseNoSlicing* b_ptr   = &b;
    BaseNoSlicing* b_ptr_d = &d;

    ASSERT_STREQ("BaseNoSlicing", b_ptr->Name());
    ASSERT_STREQ("DerivedNoSlicing", b_ptr_d->Name());

#if 0
    *b_ptr = *b_ptr_d;                // コピー演算子をdeleteしたのでコンパイルエラー
#else
    auto b_uptr = b_ptr_d->Clone();  // コピー演算子の代わりにClone()を使う。
#endif

    ASSERT_STREQ("DerivedNoSlicing", b_uptr->Name());  // 意図通り"DerivedNoSlicing"が返る。
}
// @@@ sample end
}  // namespace

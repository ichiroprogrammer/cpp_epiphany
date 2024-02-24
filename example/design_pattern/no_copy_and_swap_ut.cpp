#include <memory>

#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace {
// @@@ sample begin 0:0

class NoCopyAndSwap final {
public:
    explicit NoCopyAndSwap(char const* name0, char const* name1)
        : name0_{name0 == nullptr ? "" : name0}, name1_{name1 == nullptr ? "" : name1}
    {
    }

    NoCopyAndSwap(NoCopyAndSwap const& rhs) : name0_{rhs.name0_}, name1_{rhs.name1_} {}

    NoCopyAndSwap(NoCopyAndSwap&& rhs) noexcept
        : name0_{std::exchange(rhs.name0_, nullptr)}, name1_{std::move(rhs.name1_)}
    {
        // move後には、
        //  * name0_はnullptr
        //  * name1_はnullptrを保持したunique_ptr
        // となる。
    }

    NoCopyAndSwap& operator=(NoCopyAndSwap const& rhs)
    {
        if (this == &rhs) {
            return *this;
        }

        // copyコンストラクタのコードクローン
        name0_ = rhs.name0_;
        name1_ = rhs.name1_;  // ここでエクセプションが発生すると*thisが壊れる

        return *this;
    }

    NoCopyAndSwap& operator=(NoCopyAndSwap&& rhs) noexcept
    {
        if (this == &rhs) {
            return *this;
        }

        // moveコンストラクタのコードクローン
        name0_ = std::exchange(rhs.name0_, nullptr);
        name1_ = std::string{};  // これがないと、name1_の値がrhs.name1_にスワップされる
        name1_ = std::move(rhs.name1_);

        return *this;
    }

    char const*        GetName0() const noexcept { return name0_; }
    std::string const& GetName1() const noexcept { return name1_; }
    ~NoCopyAndSwap() = default;

private:
    char const* name0_;  // 問題やその改善を明示するために、敢えてname0_をchar const*としたが、
                         // 本来ならば、std::stringかstd::string_viewを使うべき
    std::string name1_;
};
// @@@ sample end

TEST(NoCopyAndSwap, Init)
{
    NoCopyAndSwap n{nullptr, nullptr};
    ASSERT_STREQ("", n.GetName0());
    ASSERT_EQ("", n.GetName1());

    NoCopyAndSwap a{"a0", "a1"};
    ASSERT_STREQ("a0", a.GetName0());
    ASSERT_EQ("a1", a.GetName1());
}

SUPPRESS_WARN_BEGIN;
SUPPRESS_WARN_CLANG_SELF_ASSIGN_OVERLOADED;

TEST(NoCopyAndSwap, Copy)
{
    NoCopyAndSwap const a{"a0", "a1"};

    // test for NoCopyAndSwap{NoCopyAndSwap const& rhs}
    NoCopyAndSwap b{a};

    ASSERT_STREQ("a0", b.GetName0());
    ASSERT_EQ("a1", b.GetName1());

    // test for NoCopyAndSwap& operator=(NoCopyAndSwap const& rhs)
    NoCopyAndSwap const c{"c0", "c1"};

    b = c;
    ASSERT_STREQ("c0", b.GetName0());
    ASSERT_EQ("c1", b.GetName1());

    b = b;
    ASSERT_STREQ("c0", b.GetName0());
    ASSERT_EQ("c1", b.GetName1());
}

SUPPRESS_WARN_CLANG_SELF_MOVE;

TEST(NoCopyAndSwap, Move)
{
    NoCopyAndSwap a{"a0", "a1"};

    // test for NoCopyAndSwap(NoCopyAndSwap&& rhs) noexcept
    NoCopyAndSwap b{std::move(a)};

    ASSERT_STREQ("a0", b.GetName0());
    ASSERT_EQ("a1", b.GetName1());

#if !defined(__clang_analyzer__)  // ドキュメントの都合上、scan-buildで指摘されたくない
    ASSERT_EQ(nullptr, a.GetName0());
    ASSERT_EQ("", a.GetName1());
#endif

    NoCopyAndSwap const const_a{"const_a0", "const_a1"};

    NoCopyAndSwap c{std::move(const_a)};  // moveに見えるが実はコピー

    ASSERT_STREQ("const_a0", const_a.GetName0());
    ASSERT_EQ("const_a1", const_a.GetName1());

    ASSERT_STREQ("const_a0", c.GetName0());
    ASSERT_EQ("const_a1", c.GetName1());

    // test for NoCopyAndSwap& operator=(NoCopyAndSwap&& rhs) noexcept
    c = std::move(b);
    ASSERT_STREQ("a0", c.GetName0());
    ASSERT_EQ("a1", c.GetName1());

#if !defined(__clang_analyzer__)  // ドキュメントの都合上、scan-buildで指摘されたくない
    ASSERT_EQ(nullptr, b.GetName0());
    ASSERT_EQ("", b.GetName1());
#endif

    c = std::move(c);
    ASSERT_STREQ("a0", c.GetName0());
    ASSERT_EQ("a1", c.GetName1());
}
SUPPRESS_WARN_END;
}  // namespace

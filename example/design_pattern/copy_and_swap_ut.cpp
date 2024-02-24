#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace {
// @@@ sample begin 0:0

class CopyAndSwap final {
public:
    explicit CopyAndSwap(char const* name0, char const* name1)
        : name0_{name0 == nullptr ? "" : name0}, name1_{name1 == nullptr ? "" : name1}
    {
    }

    CopyAndSwap(CopyAndSwap const& rhs) : name0_{rhs.name0_}, name1_{rhs.name1_} {}

    CopyAndSwap(CopyAndSwap&& rhs) noexcept
        : name0_{std::exchange(rhs.name0_, nullptr)}, name1_{std::move(rhs.name1_)}
    {
        // move後には、
        //  * name0_はnullptr
        //  * name1_は""を保持したstd::string
        // となる。
    }

    CopyAndSwap& operator=(CopyAndSwap const& rhs)
    {
        if (this == &rhs) {
            return *this;
        }

        // copyコンストラクタの使用
        CopyAndSwap tmp{rhs};  // ここでエクセプションが発生しても、tmp以外、壊れない

        Swap(tmp);

        return *this;
    }

    CopyAndSwap& operator=(CopyAndSwap&& rhs) noexcept
    {
        if (this == &rhs) {
            return *this;
        }

        CopyAndSwap tmp{std::move(rhs)};  // moveコンストラクタ

        Swap(tmp);

        return *this;
    }

    void Swap(CopyAndSwap& rhs) noexcept
    {
        std::swap(name0_, rhs.name0_);
        std::swap(name1_, rhs.name1_);
    }

    char const*        GetName0() const noexcept { return name0_; }
    std::string const& GetName1() const noexcept { return name1_; }
    ~CopyAndSwap() = default;

private:
    char const* name0_;  // 問題やその改善を明示するために、敢えてname0_をchar const*としたが、
                         // 本来ならば、std::stringかstd::string_viewを使うべき
    std::string name1_;
};
// @@@ sample end

TEST(CopyAndSwap, Init)
{
    CopyAndSwap n{nullptr, nullptr};
    ASSERT_STREQ("", n.GetName0());
    ASSERT_EQ("", n.GetName1());

    CopyAndSwap a{"a0", "a1"};
    ASSERT_STREQ("a0", a.GetName0());
    ASSERT_EQ("a1", a.GetName1());
}

TEST(CopyAndSwap, Swap)
{
    CopyAndSwap a{"a0", "a1"};
    CopyAndSwap b{"b0", "b1"};

    a.Swap(b);
    ASSERT_STREQ("b0", a.GetName0());
    ASSERT_EQ("b1", a.GetName1());
    ASSERT_STREQ("a0", b.GetName0());
    ASSERT_EQ("a1", b.GetName1());

    a.Swap(a);
    ASSERT_STREQ("b0", a.GetName0());
    ASSERT_EQ("b1", a.GetName1());
}

SUPPRESS_WARN_BEGIN;
SUPPRESS_WARN_CLANG_SELF_ASSIGN_OVERLOADED;

TEST(CopyAndSwap, Copy)
{
    CopyAndSwap const a{"a0", "a1"};

    // test for CopyAndSwap{CopyAndSwap const& rhs}
    CopyAndSwap b{a};

    ASSERT_STREQ("a0", b.GetName0());
    ASSERT_EQ("a1", b.GetName1());

    // test for CopyAndSwap& operator=(CopyAndSwap const& rhs)
    CopyAndSwap const c{"c0", "c1"};

    b = c;
    ASSERT_STREQ("c0", b.GetName0());
    ASSERT_EQ("c1", b.GetName1());

    b = b;
    ASSERT_STREQ("c0", b.GetName0());
    ASSERT_EQ("c1", b.GetName1());
}

SUPPRESS_WARN_CLANG_SELF_MOVE;

TEST(CopyAndSwap, Move)
{
    CopyAndSwap a{"a0", "a1"};

    // test for CopyAndSwap(CopyAndSwap&& rhs) noexcept
    CopyAndSwap b{std::move(a)};

    ASSERT_STREQ("a0", b.GetName0());
    ASSERT_EQ("a1", b.GetName1());

#if !defined(__clang_analyzer__)  // ドキュメントの都合上、scan-buildで指摘されたくない
    ASSERT_EQ(nullptr, a.GetName0());
    ASSERT_EQ("", a.GetName1());
#endif

    CopyAndSwap const const_a{"const_a0", "const_a1"};

    CopyAndSwap c{std::move(const_a)};  // moveに見えるが実はコピー

    ASSERT_STREQ("const_a0", const_a.GetName0());
    ASSERT_EQ("const_a1", const_a.GetName1());

    ASSERT_STREQ("const_a0", c.GetName0());
    ASSERT_EQ("const_a1", c.GetName1());

    // test for CopyAndSwap& operator=(CopyAndSwap&& rhs) noexcept
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

#include "gtest_wrapper.h"

#include "mpool_fixed.h"
#include "suppress_warning.h"

namespace {
TEST(NewDelete, chunk)
{
    // @@@ sample begin 0:0

    Inner_::mem_chunk<5> mc5[3];
    static_assert(32 == sizeof(mc5[0]));
    static_assert(96 == sizeof(mc5));

    auto mc33 = Inner_::mem_chunk<33>{};
    static_assert(64 == sizeof(mc33));
    // @@@ sample end
}

void test_mpf_5_3(MPoolFixed<5, 3>& mpf, bool first) noexcept
{
    ASSERT_EQ(32, mpf.GetSize());
    ASSERT_EQ(3, mpf.GetCount());
    ASSERT_FALSE(mpf.IsValid(&mpf));

    if (first) {
        ASSERT_EQ(3, mpf.GetCountMin());
    }
    else {
        ASSERT_EQ(0, mpf.GetCountMin());
    }

    auto m0 = mpf.Alloc(32);
    ASSERT_TRUE(mpf.IsValid(m0));
    ASSERT_NE(nullptr, m0);
    ASSERT_EQ(2, mpf.GetCount());

    if (first) {
        ASSERT_EQ(2, mpf.GetCountMin());
    }
    else {
        ASSERT_EQ(0, mpf.GetCountMin());
    }

    auto m1 = mpf.Alloc(32);
    ASSERT_TRUE(mpf.IsValid(m1));
    ASSERT_NE(nullptr, m1);
    ASSERT_EQ(1, mpf.GetCount());

    if (first) {
        ASSERT_EQ(1, mpf.GetCountMin());
    }
    else {
        ASSERT_EQ(0, mpf.GetCountMin());
    }

    auto m2 = mpf.Alloc(32);
    ASSERT_TRUE(mpf.IsValid(m2));
    ASSERT_NE(nullptr, m2);
    ASSERT_EQ(0, mpf.GetCount());
    ASSERT_EQ(0, mpf.GetCountMin());

    {
        ASSERT_THROW(mpf.Alloc(32), MPoolBadAlloc);
        auto m3 = mpf.AllocNoExcept(32);
        ASSERT_EQ(nullptr, m3);
        ASSERT_EQ(0, mpf.GetCount());
    }
    {
        auto except_occured = false;
        try {
            auto p = mpf.Alloc(32);
            IGNORE_UNUSED_VAR(p);
        }
        catch (MPoolBadAlloc const& e) {
            except_occured = true;
            std::cout << e.what() << std::endl;
        }
        ASSERT_TRUE(except_occured);
    }

    mpf.Free(m2);
    ASSERT_EQ(1, mpf.GetCount());
    ASSERT_EQ(0, mpf.GetCountMin());

    mpf.Free(m1);
    ASSERT_EQ(2, mpf.GetCount());
    ASSERT_EQ(0, mpf.GetCountMin());

    mpf.Free(m0);
    ASSERT_EQ(3, mpf.GetCount());
    ASSERT_EQ(0, mpf.GetCountMin());
}

TEST(NewDelete, mpool_alloc_free)
{
    {
        auto mpf = MPoolFixed<5, 3>{};

        test_mpf_5_3(mpf, true);
        test_mpf_5_3(mpf, false);
        test_mpf_5_3(mpf, false);
    }
    {
        // @@@ sample begin 1:0

        auto mpf = MPoolFixed<33, 2>{};

        ASSERT_EQ(64, mpf.GetSize());
        ASSERT_EQ(2, mpf.GetCount());
        ASSERT_EQ(2, mpf.GetCountMin());
        ASSERT_FALSE(mpf.IsValid(&mpf));  // mpfの管理外のアドレス

        auto m0 = mpf.Alloc(1);
        ASSERT_TRUE(mpf.IsValid(m0));  // mpfの管理のアドレス
        ASSERT_NE(nullptr, m0);
        ASSERT_EQ(1, mpf.GetCount());
        ASSERT_EQ(1, mpf.GetCountMin());

        auto m1 = mpf.Alloc(1);
        ASSERT_TRUE(mpf.IsValid(m1));  // mpfの管理のアドレス
        ASSERT_NE(nullptr, m1);
        ASSERT_EQ(0, mpf.GetCount());
        ASSERT_EQ(0, mpf.GetCountMin());

        // mpfが空の場合のテスト
        ASSERT_THROW(mpf.Alloc(1), MPoolBadAlloc);  // MPoolBadAlloc例外が発生するはず
        auto m2 = mpf.AllocNoExcept(1);
        ASSERT_EQ(nullptr, m2);
        ASSERT_EQ(0, mpf.GetCount());

        mpf.Free(m0);
        ASSERT_EQ(1, mpf.GetCount());
        ASSERT_EQ(0, mpf.GetCountMin());

        mpf.Free(m1);
        ASSERT_EQ(2, mpf.GetCount());
        ASSERT_EQ(0, mpf.GetCountMin());

        ASSERT_THROW(mpf.Alloc(65), MPoolBadAlloc);  // MPoolBadAlloc例外が発生するはず
        // @@@ sample end
    }
}
}  // namespace

#include <algorithm>
#include <memory>

#include "gtest_wrapper.h"

#include "global_new_delete.h"
#include "utils.h"

#ifdef SANITIZER  // sanitizerはnew/deleteオーバーロードのテストは誤動作する
#define NewDelete_Opt DISABLED_NewDelete
#else
#define NewDelete_Opt NewDelete
#endif

namespace {
TEST(NewDelete, global_new_delete_32)
{
    auto gnd = GlobalNewDeleteMonitor{};

    auto mp32 = std::find_if(gnd.cbegin(), gnd.cend(),
                             [](auto it) noexcept { return it->GetSize() == 32; });

    ASSERT_NE(mp32, gnd.cend());
    auto count = (*mp32)->GetCount();

    {
        auto i = std::make_unique<int>();
        ASSERT_EQ(count - 1, (*mp32)->GetCount());
    }
    ASSERT_EQ(count, (*mp32)->GetCount());
}

TEST(NewDelete_Opt, global_new_delete_64)
{
    auto gnd = GlobalNewDeleteMonitor{};

    auto mp64 = std::find_if(gnd.cbegin(), gnd.cend(),
                             [](auto it) noexcept { return it->GetSize() == 64; });

    ASSERT_NE(mp64, gnd.cend());
    auto count = (*mp64)->GetCount();

    {
        auto i = std::make_unique<char[]>(33);
        ASSERT_EQ(count - 1, (*mp64)->GetCount());
    }
    ASSERT_EQ(count, (*mp64)->GetCount());
}

TEST(NewDelete, global_new_delete_empty_0)
{
    auto gnd = GlobalNewDeleteMonitor{};

    auto mp32    = std::find_if(gnd.cbegin(), gnd.cend(),
                                [](auto it) noexcept { return it->GetSize() == 32; });
    auto count32 = (*mp32)->GetCount();

    auto mp64    = std::find_if(gnd.cbegin(), gnd.cend(),
                                [](auto it) noexcept { return it->GetSize() == 64; });
    auto count64 = (*mp64)->GetCount();

    {
        std::unique_ptr<char> mem[1024]{};
        auto                  fill = 0U;

        for (; fill < ArrayLength(mem); ++fill) {
            if ((*mp32)->GetCount() == 0) {
                break;
            }
            mem[fill] = std::make_unique<char>();
        }
        ASSERT_EQ(fill, count32);

        {
            // newのmpoolが切り替わるはず
            auto mem2 = std::make_unique<char>();

            ASSERT_EQ(count64 - 1, (*mp64)->GetCount());
            ASSERT_FALSE((*mp32)->IsValid(mem2.get()));
            ASSERT_TRUE((*mp64)->IsValid(mem2.get()));
        }
        ASSERT_EQ(count64, (*mp64)->GetCount());  // mem2は解放されたはず

        for (auto i = 0U; i < fill; ++i) {
            mem[i].reset();
            ASSERT_EQ(i + 1, (*mp32)->GetCount());
            ASSERT_EQ(count64, (*mp64)->GetCount());
        }
    }

    ASSERT_EQ(count32, (*mp32)->GetCount());
}

TEST(NewDelete_Opt, global_new_delete_empty_1)
{
    auto gnd = GlobalNewDeleteMonitor{};

    // テスト対象のグローバルnewが持っているMPoolのメモリ最大長は1024バイト
    auto mp1024    = std::find_if(gnd.cbegin(), gnd.cend(),
                                  [](auto it) noexcept { return it->GetSize() == 1024; });
    auto count1024 = (*mp1024)->GetCount();

    {
        std::unique_ptr<char[]> mem[1024]{};
        auto                    fill = 0U;

        for (; fill < ArrayLength(mem); ++fill) {
            if ((*mp1024)->GetCount() == 0) {
                break;
            }
            mem[fill] = std::make_unique<char[]>(1024);
        }
        ASSERT_EQ(fill, count1024);

        std::unique_ptr<char[]> mem2;
        ASSERT_THROW(mem2 = std::make_unique<char[]>(1024), std::bad_alloc);

        for (auto i = 0U; i < fill; ++i) {
            mem[i].reset();
            ASSERT_EQ(i + 1, (*mp1024)->GetCount());
        }
    }
}

TEST(NewDelete, global_new_delete_show)
{
    // @@@ sample begin 0:0

    auto gm = GlobalNewDeleteMonitor{};

    std::cout << "  size current   min" << std::endl;
    std::cout << "  ------------------" << std::endl;

    for (MPool const* mp : gm) {
        std::cout << std::setw(6) << mp->GetSize() << std::setw(8) << mp->GetCount() << std::setw(6)
                  << mp->GetCountMin() << std::endl;
    }
    // @@@ sample end
}
}  // namespace

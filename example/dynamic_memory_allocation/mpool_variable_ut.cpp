#include "gtest_wrapper.h"

#include "mpool_variable.h"

namespace {
TEST(NewDelete, buffer)
{
    static_assert(0 == sizeof(Inner_::buffer_t<0>));
    static_assert(16 == sizeof(Inner_::buffer_t<1>));
    static_assert(64 == sizeof(Inner_::buffer_t<63>));
    static_assert(96 == sizeof(Inner_::buffer_t<96>));
}

TEST(NewDelete, sprit0)
{
    using Inner_::header_t;
    using Inner_::sprit;

    static_assert(16 == sizeof(header_t));

    {
        header_t h[10];
        auto     curr = h;

        curr->next    = nullptr;
        curr->n_nuits = ArrayLength(h);

        auto opt_next = std::optional<header_t*>{sprit(curr, 2)};

        ASSERT_TRUE(opt_next);
        auto next = *opt_next;

        ASSERT_EQ(next, &h[2]);
        ASSERT_EQ(next->next, nullptr);
        ASSERT_EQ(next->n_nuits, 8);

        ASSERT_EQ(curr->next, nullptr);
        ASSERT_EQ(curr->n_nuits, 2);
    }
    {
        header_t h[10];
        auto     curr = h;

        curr->next    = nullptr;
        curr->n_nuits = ArrayLength(h);

        auto opt_next = std::optional<header_t*>{sprit(curr, 9)};

        ASSERT_TRUE(opt_next);
        auto next = *opt_next;

        ASSERT_EQ(next, nullptr);
        ASSERT_EQ(curr->next, nullptr);
        ASSERT_EQ(curr->n_nuits, 10);
    }
    {
        header_t h[10];
        auto     curr = h;

        curr->next    = nullptr;
        curr->n_nuits = ArrayLength(h);

        auto opt_next = std::optional<header_t*>{sprit(curr, 10)};

        ASSERT_TRUE(opt_next);
        auto next = *opt_next;

        ASSERT_EQ(next, nullptr);
        ASSERT_EQ(curr->next, nullptr);
        ASSERT_EQ(curr->n_nuits, 10);
    }
    {
        header_t h[10];
        auto     curr = h;

        curr->next    = nullptr;
        curr->n_nuits = ArrayLength(h);

        auto opt_next = std::optional<header_t*>{sprit(curr, 11)};

        ASSERT_FALSE(opt_next);

        ASSERT_EQ(curr->next, nullptr);
        ASSERT_EQ(curr->n_nuits, 10);
    }
}

TEST(NewDelete, sprit1)
{
    using Inner_::header_t;
    using Inner_::sprit;

    {
        header_t h[10];
        auto     curr = h;

        header_t k[3];
        k[0].next    = nullptr;
        k[0].n_nuits = ArrayLength(k);

        curr->next    = k;
        curr->n_nuits = ArrayLength(h);

        auto opt_next = std::optional<header_t*>{sprit(curr, 2)};

        ASSERT_TRUE(opt_next);
        auto next = *opt_next;

        ASSERT_EQ(next, &h[2]);
        ASSERT_EQ(next->next, k);
        ASSERT_EQ(next->n_nuits, 8);

        ASSERT_EQ(curr->next, k);
        ASSERT_EQ(curr->n_nuits, 2);
    }
    {
        header_t h[10];
        auto     curr = h;

        header_t k[3];
        k[0].next    = nullptr;
        k[0].n_nuits = ArrayLength(k);

        curr->next    = k;
        curr->n_nuits = ArrayLength(h);

        auto opt_next = std::optional<header_t*>{sprit(curr, 9)};

        ASSERT_TRUE(opt_next);
        auto next = *opt_next;

        ASSERT_EQ(next, k);
        ASSERT_EQ(next->next, nullptr);
        ASSERT_EQ(next->n_nuits, 3);
        ASSERT_EQ(curr->next, k);
        ASSERT_EQ(curr->n_nuits, 10);
    }
    {
        header_t h[10];
        auto     curr = h;

        header_t k[3];
        k[0].next    = nullptr;
        k[0].n_nuits = ArrayLength(k);

        curr->next    = k;
        curr->n_nuits = ArrayLength(h);

        auto opt_next = std::optional<header_t*>{sprit(curr, 10)};

        ASSERT_TRUE(opt_next);
        auto next = *opt_next;

        ASSERT_EQ(next, k);
        ASSERT_EQ(next->next, nullptr);
        ASSERT_EQ(next->n_nuits, 3);
        ASSERT_EQ(curr->next, k);
        ASSERT_EQ(curr->n_nuits, 10);
    }
    {
        header_t h[10];
        auto     curr = h;

        header_t k[3];
        k[0].next    = nullptr;
        k[0].n_nuits = ArrayLength(k);

        curr->next    = k;
        curr->n_nuits = ArrayLength(h);

        auto opt_next = std::optional<header_t*>{sprit(curr, 11)};

        ASSERT_FALSE(opt_next);

        ASSERT_EQ(curr->next, k);
        ASSERT_EQ(curr->n_nuits, 10);
    }
}

TEST(NewDelete, concat)
{
    using Inner_::concat;
    using Inner_::header_t;

    {
        header_t h[10];

        auto h0 = h;
        auto h1 = &h[2];
        auto h2 = &h[4];

        h0->n_nuits = 2;
        h0->next    = &h[9];  // fake
        h1->n_nuits = 2;
        h1->next    = &h[10];  // fake
        h2->n_nuits = 2;
        h2->next    = nullptr;

        concat(h1, h2);
        ASSERT_EQ(h1->n_nuits, 4);
        ASSERT_EQ(h1->next, nullptr);

        concat(h0, h1);
        ASSERT_EQ(h0->n_nuits, 6);
        ASSERT_EQ(h0->next, nullptr);
    }
    {
        header_t h[10];

        auto h0 = h;
        auto h2 = &h[4];

        h0->n_nuits = 2;
        h0->next    = &h[9];  // fake
        h2->n_nuits = 2;
        h2->next    = nullptr;

        concat(h0, h2);
        ASSERT_EQ(h0->n_nuits, 2);
        ASSERT_EQ(h0->next, h2);
    }
}

TEST(NewDelete, itor)
{
    constexpr auto mem_size = 256U;
    auto           mpv      = MPoolVariable<mem_size>{};

    auto c_itor = mpv.begin();

    ASSERT_EQ((*c_itor)->next, nullptr);
    ASSERT_EQ((*c_itor)->n_nuits, mem_size / sizeof(Inner_::header_t));
    ASSERT_EQ(mpv.GetCount(), mem_size);
    ASSERT_EQ(mpv.GetCountMin(), mem_size);
    ASSERT_EQ(mpv.GetSize(), 1);

    using Inner_::header_t;
    using Inner_::set_back;

    void* ints[8]{};

    using Inner_::unit_size;
    constexpr auto n_nuits = Roundup(unit_size, unit_size + sizeof(int)) / unit_size;

    // -------
    auto count = mem_size;
    for (auto& i : ints) {
        i = mpv.Alloc(sizeof(int));
        count -= sizeof(Inner_::header_t) * n_nuits;
        ASSERT_EQ(mpv.GetCount(), count);
        ASSERT_EQ(mpv.GetCountMin(), count);

        header_t* h = set_back(i);
        ASSERT_EQ(h->n_nuits, n_nuits);
    }

    ASSERT_THROW(mpv.Alloc(sizeof(int)), MPoolBadAlloc);
    ASSERT_EQ(mpv.AllocNoExcept(sizeof(int)), nullptr);

    count = 0;
    for (auto& i : ints) {
        mpv.Free(i);
        count += sizeof(Inner_::header_t) * n_nuits;
        ASSERT_EQ(mpv.GetCount(), count);
        ASSERT_EQ(mpv.GetCountMin(), 0);
    }

    // ------- フラグメントテスト
    {
        auto flagments = 0U;
        for (auto itor = mpv.cbegin(); itor != mpv.cend(); ++itor) {
            // std::cout << "addr:" << *itor << ":" << (*itor)->next << ":"
            //           << (*itor)->n_nuits << std::endl;
            flagments += 1;
        }
        ASSERT_EQ(1, flagments);
    }

    for (auto& i : ints) {
        i = mpv.Alloc(sizeof(int));
    }

    {
        auto flagments = 0U;
        for (auto itor = mpv.cbegin(); itor != mpv.cend(); ++itor) {
            flagments += 1;
        }
        ASSERT_EQ(0, flagments);
    }

    count = 0;
    for (auto i = 0U; i < ArrayLength(ints); i += 2) {
        mpv.Free(ints[i]);
        count += sizeof(Inner_::header_t) * n_nuits;
        ASSERT_EQ(mpv.GetCount(), count);
    }

    {
        auto flagments = 0U;
        for (auto itor = mpv.cbegin(); itor != mpv.cend(); ++itor) {
            flagments += 1;
        }
        ASSERT_EQ(4, flagments);
    }

    ASSERT_THROW(mpv.Alloc(sizeof(char[17])), MPoolBadAlloc);
    ASSERT_EQ(mpv.AllocNoExcept(sizeof(char[17])), nullptr);
    for (auto i = 1U; i < ArrayLength(ints); i += 2) {
        ints[i] = mpv.Alloc(sizeof(char[16]));
        ASSERT_NE(nullptr, ints[i]);
        count -= sizeof(Inner_::header_t) * n_nuits;
        ASSERT_EQ(mpv.GetCount(), count);
    }
}
}  // namespace

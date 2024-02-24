#include <cassert>
#include <new>

#include "global_new_delete.h"
#include "mpool_fixed.h"

// @@@ sample begin 0:0

namespace {

constexpr size_t min_unit{MPoolFixed_MinSize};

template <uint32_t N_UNITS, uint32_t MEM_COUNT>
[[nodiscard]] MPool* gen_mpool() noexcept
{
    using mp_t = MPoolFixed<min_unit * N_UNITS, MEM_COUNT>;

    static union {
        std::max_align_t max_align;
        uint8_t          mem[sizeof(mp_t)];
    } mem;

    static_assert(static_cast<void*>(&mem.max_align) == static_cast<void*>(mem.mem));
    static_assert(sizeof(mem) >= sizeof(mp_t));

    return new (mem.mem) mp_t;  // プレースメントnew
}
}  // namespace
// @@@ sample end
// @@@ sample begin 1:0

namespace {
MPool* mpool_table[32];

// mainの前に呼ばれるため、mpool_tableを初期化するには下記のような方法が必要
bool is_setup{false};

void setup() noexcept
{
    is_setup = true;

    mpool_table[0] = gen_mpool<1, 128>();  //   32
    mpool_table[1] = gen_mpool<2, 128>();  //   64
    mpool_table[2] = gen_mpool<3, 128>();  //   96

    // @@@ ignore begin
    mpool_table[3]  = gen_mpool<4, 128>();   //  128
    mpool_table[4]  = gen_mpool<5, 128>();   //  160
    mpool_table[5]  = gen_mpool<6, 128>();   //  192
    mpool_table[6]  = gen_mpool<7, 128>();   //  224
    mpool_table[7]  = gen_mpool<8, 128>();   //  256
    mpool_table[8]  = gen_mpool<9, 128>();   //  288
    mpool_table[9]  = gen_mpool<10, 128>();  //  320
    mpool_table[10] = gen_mpool<11, 128>();  //  352
    mpool_table[11] = gen_mpool<12, 128>();  //  384
    mpool_table[12] = gen_mpool<13, 128>();  //  416
    mpool_table[13] = gen_mpool<14, 128>();  //  448
    mpool_table[14] = gen_mpool<15, 128>();  //  490
    mpool_table[15] = gen_mpool<16, 128>();  //  512
    mpool_table[16] = gen_mpool<17, 128>();  //  544
    mpool_table[17] = gen_mpool<18, 128>();  //  576
    mpool_table[18] = gen_mpool<19, 128>();  //  608
    mpool_table[19] = gen_mpool<20, 128>();  //  640
    mpool_table[20] = gen_mpool<21, 128>();  //  672
    mpool_table[21] = gen_mpool<22, 128>();  //  704
    mpool_table[22] = gen_mpool<23, 128>();  //  736
    mpool_table[23] = gen_mpool<24, 128>();  //  768
    mpool_table[24] = gen_mpool<25, 128>();  //  800
    mpool_table[25] = gen_mpool<26, 128>();  //  832
    mpool_table[26] = gen_mpool<27, 128>();  //  864
    mpool_table[27] = gen_mpool<28, 128>();  //  896
    mpool_table[28] = gen_mpool<29, 128>();  //  928
    // @@@ ignore end

    mpool_table[29] = gen_mpool<30, 128>();  //  960
    mpool_table[30] = gen_mpool<31, 128>();  //  992
    mpool_table[31] = gen_mpool<32, 128>();  // 1024
}

size_t size2index(size_t v) noexcept
{
    return (((v + (min_unit - 1)) & ~(min_unit - 1)) / min_unit) - 1;
}
}  // namespace
// @@@ sample end
// @@@ sample begin 2:0

[[nodiscard]] void* operator new(std::size_t size)
{
    if (!is_setup) {
        setup();
    }

    for (auto i = size2index(size); i < ArrayLength(mpool_table); ++i) {
        void* mem = mpool_table[i]->AllocNoExcept(size);
        if (mem != nullptr) {
            return mem;
        }
    }

    throw std::bad_alloc{};

    static char fake[0];

    return fake;
}
// @@@ sample end
// @@@ sample begin 3:0

void operator delete(void* mem) noexcept
{
    for (MPool* mp : mpool_table) {
        if (mp->IsValid(mem)) {
            mp->Free(mem);
            return;
        }
    }
    assert(false);
}
// @@@ sample end
// @@@ sample begin 4:0

void operator delete(void* mem, std::size_t size) noexcept
{
    for (auto i = size2index(size); i < ArrayLength(mpool_table); ++i) {
        if (mpool_table[i]->IsValid(mem)) {
            mpool_table[i]->Free(mem);
            return;
        }
    }
    assert(false);
}
// @@@ sample end
// @@@ sample begin 5:0

MPool const* const* GlobalNewDeleteMonitor::begin() const noexcept { return &mpool_table[0]; }
MPool const* const* GlobalNewDeleteMonitor::end() const noexcept
{
    return &mpool_table[ArrayLength(mpool_table)];
}

MPool const* const* GlobalNewDeleteMonitor::cbegin() const noexcept { return begin(); }
MPool const* const* GlobalNewDeleteMonitor::cend() const noexcept { return end(); }
// @@@ sample end

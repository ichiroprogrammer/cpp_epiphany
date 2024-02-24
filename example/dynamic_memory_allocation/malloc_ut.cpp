#include <sys/unistd.h>

#include <cassert>
#include <cstdint>
#include <mutex>

#include "gtest_wrapper.h"

#include "spin_lock.h"
#include "utils.h"

// @@@ sample begin 0:0

extern "C" void* sbrk(ptrdiff_t __incr);
// @@@ sample end

namespace MallocFree {
// @@@ sample begin 1:0

namespace {

struct header_t {
    header_t* next;
    size_t    n_nuits;  // header_tが何個あるか
};

header_t*        header{nullptr};
SpinLock         spin_lock{};
constexpr size_t unit_size{sizeof(header_t)};

inline bool sprit(header_t* header, size_t n_nuits, header_t*& next) noexcept
{
    // @@@ ignore begin
    assert(n_nuits > 1);  // ヘッダとバッファなので最低でも2

    next = nullptr;

    if (header->n_nuits == n_nuits || header->n_nuits == n_nuits + 1) {
        next = header->next;
        return true;
    }
    else if (header->n_nuits > n_nuits) {
        next            = header + n_nuits;
        next->n_nuits   = header->n_nuits - n_nuits;
        next->next      = header->next;
        header->n_nuits = n_nuits;
        return true;
    }

    return false;
    // @@@ ignore end
}

inline void concat(header_t* front, header_t* rear) noexcept
{
    // @@@ ignore begin
    if (front + front->n_nuits == rear) {  // 1枚のメモリになる
        front->n_nuits += rear->n_nuits;
        front->next = rear->next;
    }
    else {
        front->next = rear;
    }
    // @@@ ignore end
}

header_t* set_back(void* mem) noexcept { return static_cast<header_t*>(mem) - 1; }

static_assert(sizeof(header_t) == alignof(std::max_align_t));

void* malloc_inner(size_t size) noexcept
{
    // @@@ ignore begin
    // size分のメモリとヘッダ
    auto n_nuits = (Roundup(unit_size, size) / unit_size) + 1;
    auto lock    = std::lock_guard{spin_lock};

    auto curr = header;
    for (header_t* prev = nullptr; curr != nullptr; prev = curr, curr = curr->next) {
        header_t* next;

        if (!sprit(curr, n_nuits, next)) {
            continue;
        }

        if (prev == nullptr) {
            header = next;
        }
        else {
            prev->next = next;
        }
        break;
    }

    if (curr != nullptr) {
        ++curr;
    }

    return curr;
    // @@@ ignore end
}
}  // namespace
// @@@ sample end
// @@@ sample begin 2:0

void free(void* mem) noexcept
{
    header_t* mem_to_free = set_back(mem);

    mem_to_free->next = nullptr;

    auto lock = std::lock_guard{spin_lock};

    if (header == nullptr) {
        header = mem_to_free;
        return;
    }
    // @@@ sample end
    // @@@ sample begin 2:1

    if (mem_to_free < header) {
        concat(mem_to_free, header);
        header = mem_to_free;
        return;
    }

    auto curr = header;
    for (; curr->next != nullptr; curr = curr->next) {
        if (mem_to_free < curr->next) {  // 常に curr < mem_to_free
            concat(mem_to_free, curr->next);
            concat(curr, mem_to_free);
            return;
        }
    }

    concat(curr, mem_to_free);
    // @@@ sample end
    // @@@ sample begin 2:2
}
// @@@ sample end
// @@@ sample begin 3:0

void* malloc(size_t size) noexcept
{
    void* mem = malloc_inner(size);
    // @@@ sample end
    // @@@ sample begin 3:1

    if (mem == nullptr) {
        auto const add_size = Roundup(unit_size, 1024 * 1024 + size);  // 1MB追加

        header_t* add = static_cast<header_t*>(sbrk(add_size));
        add->n_nuits  = add_size / unit_size;
        free(++add);
        mem = malloc_inner(size);
    }
    // @@@ sample end
    // @@@ sample begin 3:2

    return mem;
}
// @@@ sample end

namespace {
TEST(NewDelete, malloc)
{
    {
        void* mem = malloc(1024);

        ASSERT_NE(nullptr, mem);
        free(mem);

        void* ints[8]{};

        constexpr auto n_nuits = Roundup(unit_size, unit_size + sizeof(int)) / unit_size;

        for (auto& i : ints) {
            i = malloc(sizeof(int));

            header_t* h = set_back(i);
            ASSERT_EQ(h->n_nuits, n_nuits);
        }

        for (auto& i : ints) {
            free(i);
        }
    }

    // @@@ sample begin 4:0

    void* mem[1024];

    for (auto& m : mem) {  // 32バイト x 1024個のメモリ確保
        m = malloc(32);
    }

    // memを使用した何らかの処理
    // @@@ ignore begin
    // @@@ ignore end

    for (auto i = 0U; i < ArrayLength(mem); i += 2) {  // 512個のメモリを解放
        free(mem[i]);
    }
    // @@@ sample end

    for (auto i = 1U; i < ArrayLength(mem); i += 2) {
        free(mem[i]);
    }
}
}  // namespace
}  // namespace MallocFree

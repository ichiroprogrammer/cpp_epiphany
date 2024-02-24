#pragma once
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <mutex>

#include "mpool.h"
#include "spin_lock.h"
#include "utils.h"

// @@@ sample begin 0:0

constexpr size_t MPoolFixed_MinSize{32};

namespace Inner_ {
template <uint32_t MEM_SIZE>
union mem_chunk {
    mem_chunk* next;

    // MPoolFixed_MinSizeの整数倍のエリアを、最大アラインメントが必要な基本型にアライン
    alignas(alignof(std::max_align_t)) uint8_t mem[Roundup(MPoolFixed_MinSize, MEM_SIZE)];
};
}  // namespace Inner_
// @@@ sample end
// @@@ sample begin 1:0

template <uint32_t MEM_SIZE, uint32_t MEM_COUNT>
class MPoolFixed final : public MPool {
public:
    MPoolFixed() noexcept : MPool{mem_chunk_size_} {}

private:
    using chunk_t = Inner_::mem_chunk<MEM_SIZE>;
    static constexpr size_t mem_chunk_size_{sizeof(chunk_t)};

    size_t           mem_count_{MEM_COUNT};
    size_t           mem_count_min_{MEM_COUNT};
    chunk_t          mem_chunk_[MEM_COUNT]{};
    chunk_t*         mem_head_{setup_mem()};
    mutable SpinLock spin_lock_{};

    chunk_t* setup_mem() noexcept
    {
        for (auto i = 0U; i < MEM_COUNT - 1; ++i) {
            mem_chunk_[i].next = &mem_chunk_[i + 1];
        }

        mem_chunk_[MEM_COUNT - 1].next = nullptr;

        return mem_chunk_;
    }

    virtual void* alloc(size_t size) noexcept override
    {
        assert(size <= mem_chunk_size_);

        auto lock = std::lock_guard{spin_lock_};

        auto mem = mem_head_;

        if (mem != nullptr) {
            mem_head_      = mem_head_->next;
            mem_count_min_ = std::min(--mem_count_, mem_count_min_);
        }

        return mem;
    }

    virtual void free(void* mem) noexcept override
    {
        assert(is_valid(mem));

        auto lock = std::lock_guard{spin_lock_};

        chunk_t* curr_head = static_cast<chunk_t*>(mem);
        curr_head->next    = mem_head_;
        mem_head_          = curr_head;

        mem_count_min_ = std::min(++mem_count_, mem_count_min_);
    }

    virtual size_t get_size() const noexcept override { return mem_chunk_size_; }
    virtual size_t get_count() const noexcept override { return mem_count_; }
    virtual size_t get_count_min() const noexcept override { return mem_count_min_; }

    virtual bool is_valid(void const* mem) const noexcept override
    {
        return (&mem_chunk_[0] <= mem) && (mem <= &mem_chunk_[MEM_COUNT - 1]);
    }
};
// @@@ sample end

#pragma once
#include <cassert>
#include <cstdint>
#include <mutex>
#include <optional>

#include "mpool.h"
#include "spin_lock.h"
#include "utils.h"

namespace Inner_ {

struct header_t {
    header_t* next;
    size_t    n_nuits;  // header_tが何個あるか
};

constexpr auto unit_size = sizeof(header_t);

inline std::optional<header_t*> sprit(header_t* header, size_t n_nuits) noexcept
{
    assert(n_nuits > 1);  // ヘッダとバッファなので最低でも2

    if (header->n_nuits == n_nuits || header->n_nuits == n_nuits + 1) {
        return header->next;
    }
    else if (header->n_nuits > n_nuits) {
        auto next       = header + n_nuits;
        next->n_nuits   = header->n_nuits - n_nuits;
        next->next      = header->next;
        header->n_nuits = n_nuits;
        return next;
    }

    return std::nullopt;
}

inline void concat(header_t* front, header_t* rear) noexcept
{
    if (front + front->n_nuits == rear) {  // 1枚のメモリになる
        front->n_nuits += rear->n_nuits;
        front->next = rear->next;
    }
    else {
        front->next = rear;
    }
}

inline header_t* set_back(void* mem) noexcept { return static_cast<header_t*>(mem) - 1; }

static_assert(sizeof(header_t) == alignof(std::max_align_t));

template <uint32_t MEM_SIZE>
struct buffer_t {
    alignas(alignof(std::max_align_t)) uint8_t buffer[Roundup(sizeof(header_t), MEM_SIZE)];
};
}  // namespace Inner_

// @@@ sample begin 0:0

template <uint32_t MEM_SIZE>
class MPoolVariable final : public MPool {
public:
    // @@@ sample end
    // @@@ sample begin 0:1
    MPoolVariable() noexcept : MPool{MEM_SIZE}
    {
        header_->next    = nullptr;
        header_->n_nuits = sizeof(buff_) / Inner_::unit_size;
    }
    // @@@ sample end
    // @@@ sample begin 0:2

    class const_iterator {
    public:
        explicit const_iterator(Inner_::header_t const* header) noexcept : header_{header} {}
        const_iterator(const_iterator const&) = default;
        const_iterator(const_iterator&&)      = default;

        const_iterator& operator++() noexcept  // 前置++のみ実装
        {
            assert(header_ != nullptr);
            header_ = header_->next;

            return *this;
        }

        Inner_::header_t const* operator*() noexcept { return header_; }
        bool operator==(const_iterator const& rhs) noexcept { return header_ == rhs.header_; }
        bool operator!=(const_iterator const& rhs) noexcept { return !(*this == rhs); }

    private:
        Inner_::header_t const* header_;
    };

    const_iterator begin() const noexcept { return const_iterator{header_}; }
    const_iterator end() const noexcept { return const_iterator{nullptr}; }
    const_iterator cbegin() const noexcept { return const_iterator{header_}; }
    const_iterator cend() const noexcept { return const_iterator{nullptr}; }
    // @@@ sample end
    // @@@ sample begin 0:3

private:
    using header_t = Inner_::header_t;

    Inner_::buffer_t<MEM_SIZE> buff_{};
    header_t*                  header_{reinterpret_cast<header_t*>(buff_.buffer)};
    mutable SpinLock           spin_lock_{};
    size_t                     unit_count_{sizeof(buff_) / Inner_::unit_size};
    size_t                     unit_count_min_{sizeof(buff_) / Inner_::unit_size};

    virtual void* alloc(size_t size) noexcept override
    {
        // @@@ ignore begin
        // size分のメモリとヘッダ
        auto n_nuits = (Roundup(Inner_::unit_size, size) / Inner_::unit_size) + 1;

        auto lock = std::lock_guard{spin_lock_};

        auto curr = header_;

        for (header_t* prev{nullptr}; curr != nullptr; prev = curr, curr = curr->next) {
            auto opt_next = std::optional<header_t*>{sprit(curr, n_nuits)};

            if (!opt_next) {
                continue;
            }

            auto next = *opt_next;
            if (prev == nullptr) {
                header_ = next;
            }
            else {
                prev->next = next;
            }
            break;
        }

        if (curr != nullptr) {
            unit_count_ -= curr->n_nuits;
            unit_count_min_ = std::min(unit_count_, unit_count_min_);
            ++curr;
        }

        return curr;
        // @@@ ignore end
    }

    virtual void free(void* mem) noexcept override
    {
        // @@@ ignore begin
        header_t* to_free = Inner_::set_back(mem);

        to_free->next = nullptr;

        auto lock = std::lock_guard{spin_lock_};

        unit_count_ += to_free->n_nuits;
        unit_count_min_ = std::min(unit_count_, unit_count_min_);

        if (header_ == nullptr) {
            header_ = to_free;
            return;
        }

        if (to_free < header_) {
            concat(to_free, header_);
            header_ = to_free;
            return;
        }

        header_t* curr = header_;

        for (; curr->next != nullptr; curr = curr->next) {
            if (to_free < curr->next) {  // 常に curr < to_free
                concat(to_free, curr->next);
                concat(curr, to_free);
                return;
            }
        }

        concat(curr, to_free);
        // @@@ ignore end
    }

    virtual size_t get_size() const noexcept override { return 1; }
    virtual size_t get_count() const noexcept override { return unit_count_ * Inner_::unit_size; }
    virtual size_t get_count_min() const noexcept override
    {
        return unit_count_min_ * Inner_::unit_size;
    }

    virtual bool is_valid(void const* mem) const noexcept override
    {
        return (&buff_ < mem) && (mem < &buff_.buffer[ArrayLength(buff_.buffer)]);
    }
    // @@@ sample end
    // @@@ sample begin 0:4
};
// @@@ sample end

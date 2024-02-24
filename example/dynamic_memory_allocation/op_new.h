#pragma once

#include "mpool.h"

// @@@ sample begin 0:0

template <typename T>
class OpNew {
public:
    [[nodiscard]] static void* operator new(size_t size) { return mpool_.Alloc(size); }
    static void                operator delete(void* mem) noexcept { mpool_.Free(mem); }
    static void operator delete(void* mem, std::size_t) noexcept { mpool_.Free(mem); }

    [[nodiscard]] static void* operator new[](size_t size)                             = delete;
    static void                operator delete[](void* mem) noexcept                   = delete;
    static void                operator delete[](void* mem, std::size_t size) noexcept = delete;

private:
    static MPool& mpool_;
};
// @@@ sample end

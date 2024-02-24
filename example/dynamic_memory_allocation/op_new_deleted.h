#pragma once

// @@@ sample begin 0:0

class OpNewDeleted {
    static void* operator new(size_t size)                             = delete;
    static void  operator delete(void* mem) noexcept                   = delete;
    static void  operator delete(void* mem, std::size_t size) noexcept = delete;
};
// @@@ sample end

#pragma once

#include <type_traits>

#include "mpool.h"

// @@@ sample begin 0:0

template <typename T>
class MPoolBasedAllocator {
public:
    using pointer                                = T*;
    using const_pointer                          = T const*;
    using value_type                             = T;
    using propagate_on_container_move_assignment = std::true_type;
    using is_always_equal                        = std::true_type;
    using size_type                              = size_t;
    using difference_type                        = size_t;

    template <class U>
    struct rebind {
        using other = MPoolBasedAllocator<U>;
    };

    T*   allocate(size_type count) { return static_cast<pointer>(mpool_.Alloc(count * sizeof(T))); }
    void deallocate(T* mem, size_type) noexcept { mpool_.Free(mem); }

private:
    static MPool& mpool_;
};

template <class T>  // T型のMPoolBasedAllocatorはシステムに唯一
bool operator==(MPoolBasedAllocator<T> const&, MPoolBasedAllocator<T> const&) noexcept
{
    return true;
}

template <class T, class U>
bool operator==(MPoolBasedAllocator<T> const&, MPoolBasedAllocator<U> const&) noexcept
{
    return false;
}

template <class T, class U>
bool operator!=(MPoolBasedAllocator<T> const& lhs, MPoolBasedAllocator<U> const& rhs) noexcept
{
    return !(lhs == rhs);
}
// @@@ sample end

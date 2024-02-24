#pragma once
#include "mpool.h"

// @@@ sample begin 0:0

class GlobalNewDeleteMonitor {
public:
    MPool const* const* cbegin() const noexcept;
    MPool const* const* cend() const noexcept;
    MPool const* const* begin() const noexcept;
    MPool const* const* end() const noexcept;
};
// @@@ sample end

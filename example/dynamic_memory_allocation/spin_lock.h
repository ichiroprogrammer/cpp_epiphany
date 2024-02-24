#pragma once

// @@@ sample begin 0:0

#include <atomic>

class SpinLock {
public:
    void lock() noexcept
    {
        while (state_.exchange(state::locked, std::memory_order_acquire) == state::locked) {
            ;  // busy wait
        }
    }

    void unlock() noexcept { state_.store(state::unlocked, std::memory_order_release); }

private:
    enum class state { locked, unlocked };
    std::atomic<state> state_{state::unlocked};
};
// @@@ sample end

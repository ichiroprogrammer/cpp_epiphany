#include <cassert>
#include <cstdint>

#include "state_machine_old.h"

// @@@ sample begin 0:0

namespace {
enum class ThreadOldStyleState {
    Idle,
    Running,
    Suspending,
};

ThreadOldStyleState thread_old_style_state;
// @@@ ignore begin

uint32_t thread_old_style_suspend_count;

// @@@ ignore end
}  // namespace

std::string_view ThreadOldStyleStateStr() noexcept
{
    switch (thread_old_style_state) {  // このswitch文と同型switch文が何度も記述される
    case ThreadOldStyleState::Idle:
        return "Idle";
    case ThreadOldStyleState::Running:
        return "Running";
    case ThreadOldStyleState::Suspending:
        return "Suspending";
    default:
        assert(false);
        return "";
    }
}

void ThreadOldStyleRun()
{
    switch (thread_old_style_state) {
    case ThreadOldStyleState::Idle:
    case ThreadOldStyleState::Running:
        thread_old_style_state = ThreadOldStyleState::Running;
        break;
    case ThreadOldStyleState::Suspending:
        --thread_old_style_suspend_count;
        if (thread_old_style_suspend_count == 0) {
            thread_old_style_state = ThreadOldStyleState::Running;
        }
        break;
    default:
        assert(false);
    }
}

void ThreadOldStyleAbort()
{
    // @@@ ignore begin
    switch (thread_old_style_state) {
    case ThreadOldStyleState::Idle:
        break;
    case ThreadOldStyleState::Running:
        thread_old_style_state = ThreadOldStyleState::Idle;
        break;
    case ThreadOldStyleState::Suspending:
        thread_old_style_suspend_count = 0;
        thread_old_style_state         = ThreadOldStyleState::Idle;
        break;
    default:
        assert(false);
    }
    // @@@ ignore end
}

void ThreadOldStyleSuspend()
{
    // @@@ ignore begin
    switch (thread_old_style_state) {
    case ThreadOldStyleState::Idle:
        break;
    case ThreadOldStyleState::Running:
        thread_old_style_state         = ThreadOldStyleState::Suspending;
        thread_old_style_suspend_count = 1;
        break;
    case ThreadOldStyleState::Suspending:
        ++thread_old_style_suspend_count;
        break;
    default:
        assert(false);
    }
    // @@@ ignore end
}
// @@@ sample end

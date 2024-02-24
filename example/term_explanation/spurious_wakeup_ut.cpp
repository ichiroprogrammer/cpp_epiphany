#include <condition_variable>
#include <mutex>

#include "gtest_wrapper.h"

namespace SpuriousWakeup {

// @@@ sample begin 0:0

namespace {
std::mutex              mutex;
std::condition_variable cond_var;
}  // namespace

void notify_wrong()  // 通知を行うスレッドが呼び出す関数
{
    auto lock = std::lock_guard{mutex};

    cond_var.notify_all();  // wait()で待ち状態のスレッドを起こす。
}

void wait_wrong()  // 通知待ちスレッドが呼び出す関数
{
    auto lock = std::unique_lock{mutex};

    // notifyされるのを待つ。
    cond_var.wait(lock);  // notify_allされなくても起き上がってしまうことがある。

    // do something
}
// @@@ sample end
}  // namespace SpuriousWakeup
namespace NotSpuriousWakeup {
// @@@ sample begin 1:0

namespace {
bool                    event_occured{false};
std::mutex              mutex;
std::condition_variable cond_var;
}  // namespace

void notify_right()  // 通知を行うスレッドが呼び出す関数
{
    auto lock = std::lock_guard{mutex};

    event_occured = true;

    cond_var.notify_all();  // wait()で待ち状態のスレッドを起こす。
}

void wait_right()  // 通知待ちスレッドが呼び出す関数
{
    auto lock = std::unique_lock{mutex};

    // notifyされるのを待つ。
    cond_var.wait(lock, []() noexcept { return event_occured; });  // Spurious Wakeup対策

    event_occured = false;

    // do something
}
// @@@ sample end
}  // namespace NotSpuriousWakeup

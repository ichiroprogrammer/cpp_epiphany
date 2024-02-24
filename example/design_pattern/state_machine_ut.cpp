#include <cassert>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>

#include "gtest_wrapper.h"

#include "state_machine_new.h"
#include "state_machine_old.h"

namespace {
// not_state_machine

// @@@ sample begin 0:0

// ステートのテスト。仕様書よりも単体テストでその仕様や使用法を記述したほうが正確に理解できる。
TEST(StateMachine, old_style)
{
    ASSERT_EQ("Idle", ThreadOldStyleStateStr());

    ThreadOldStyleAbort();
    ASSERT_EQ("Idle", ThreadOldStyleStateStr());

    ThreadOldStyleRun();
    ASSERT_EQ("Running", ThreadOldStyleStateStr());

    ThreadOldStyleRun();
    ASSERT_EQ("Running", ThreadOldStyleStateStr());

    ThreadOldStyleSuspend();
    ASSERT_EQ("Suspending", ThreadOldStyleStateStr());  // suspend_count_ == 1

    ThreadOldStyleSuspend();
    ASSERT_EQ("Suspending", ThreadOldStyleStateStr());  // suspend_count_ == 2

    ThreadOldStyleRun();
    ASSERT_EQ("Suspending", ThreadOldStyleStateStr());  // suspend_count_ == 1

    // @@@ ignore begin
    ThreadOldStyleRun();
    ASSERT_EQ("Running", ThreadOldStyleStateStr());

    ThreadOldStyleSuspend();
    ASSERT_EQ("Suspending", ThreadOldStyleStateStr());  // suspend_count_ == 1

    ThreadOldStyleAbort();
    ASSERT_EQ("Idle", ThreadOldStyleStateStr());

    ThreadOldStyleRun();
    ThreadOldStyleAbort();
    ASSERT_EQ("Idle", ThreadOldStyleStateStr());
    // @@@ ignore end
}
// @@@ sample end

// @@@ sample begin 1:0

TEST(StateMachine, new_style)
{
    auto tns = ThreadNewStyle{};

    ASSERT_EQ("Idle", tns.GetStateStr());

    tns.Abort();
    ASSERT_EQ("Idle", tns.GetStateStr());

    tns.Run();
    ASSERT_EQ("Running", tns.GetStateStr());

    tns.Run();
    ASSERT_EQ("Running", tns.GetStateStr());

    tns.Suspend();
    ASSERT_EQ("Suspending", tns.GetStateStr());  // suspend_count_ == 1

    tns.Suspend();
    ASSERT_EQ("Suspending", tns.GetStateStr());  // suspend_count_ == 2

    tns.Run();
    ASSERT_EQ("Suspending", tns.GetStateStr());  // suspend_count_ == 1

    // @@@ ignore begin
    tns.Run();
    ASSERT_EQ("Running", tns.GetStateStr());

    tns.Suspend();
    ASSERT_EQ("Suspending", tns.GetStateStr());  // suspend_count_ == 1

    tns.Abort();
    ASSERT_EQ("Idle", tns.GetStateStr());
    // @@@ ignore end
}
// @@@ sample end
// @@@ sample begin 2:0

TEST(StateMachine, new_style2)
{
    auto tns = ThreadNewStyle2{};

    ASSERT_EQ("Idle", tns.GetStateStr());

    tns.Run();
    ASSERT_EQ("Running", tns.GetStateStr());

    tns.Suspend();
    ASSERT_EQ("Suspending", tns.GetStateStr());

    tns.Suspend();
    ASSERT_EQ("Suspending", tns.GetStateStr());
}
// @@@ sample end
}  // namespace

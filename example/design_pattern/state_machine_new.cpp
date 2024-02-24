#include <iostream>

#include "state_machine_new.h"

#define LOG()                                                         \
    do {                                                              \
        std::cout << __LINE__ << ":" << __func__ << ":" << std::endl; \
    } while (0)

// @@@ sample begin 0:0

class ThreadNewStyleState_Idle final : public ThreadNewStyleState {
    // @@@ ignore begin
public:
    ThreadNewStyleState_Idle() { LOG(); }

    virtual ~ThreadNewStyleState_Idle() override { LOG(); }

private:
    virtual std::unique_ptr<ThreadNewStyleState> run_thread() override;
    virtual std::string_view get_state_str() const noexcept override { return "Idle"; }
    // @@@ ignore end
};

class ThreadNewStyleState_Running final : public ThreadNewStyleState {
    // @@@ ignore begin
public:
    ThreadNewStyleState_Running() { LOG(); }

    virtual ~ThreadNewStyleState_Running() override { LOG(); }

private:
    virtual std::unique_ptr<ThreadNewStyleState> abort_thread() override;
    virtual std::unique_ptr<ThreadNewStyleState> suspend_thread() override;
    virtual std::string_view get_state_str() const noexcept override { return "Running"; }
    // @@@ ignore end
};

class ThreadNewStyleState_Suspending final : public ThreadNewStyleState {
public:
    // @@@ ignore begin
    ThreadNewStyleState_Suspending() { LOG(); }

    virtual ~ThreadNewStyleState_Suspending() override { LOG(); }
    // @@@ ignore end
private:
    virtual std::unique_ptr<ThreadNewStyleState> abort_thread() override
    {
        // do something to abort
        // @@@ ignore begin
        // @@@ ignore end

        return std::make_unique<ThreadNewStyleState_Idle>();
    }

    virtual std::unique_ptr<ThreadNewStyleState> run_thread() override
    {
        --suspend_count_;

        if (suspend_count_ == 0) {
            // do something to resume
            // @@@ ignore begin
            // @@@ ignore end
            return std::make_unique<ThreadNewStyleState_Running>();
        }
        else {
            return {};
        }
    }

    virtual std::unique_ptr<ThreadNewStyleState> suspend_thread() override
    {
        ++suspend_count_;

        return {};
    }
    // @@@ ignore begin

    virtual std::string_view get_state_str() const noexcept override { return "Suspending"; }

    uint32_t suspend_count_{1};
    // @@@ ignore end
};
// @@@ sample end

std::unique_ptr<ThreadNewStyleState> ThreadNewStyleState_Idle::run_thread()
{
    return std::make_unique<ThreadNewStyleState_Running>();
}

std::unique_ptr<ThreadNewStyleState> ThreadNewStyleState_Running::abort_thread()
{
    return std::make_unique<ThreadNewStyleState_Idle>();
}

std::unique_ptr<ThreadNewStyleState> ThreadNewStyleState_Running::suspend_thread()
{
    return std::make_unique<ThreadNewStyleState_Suspending>();
}

ThreadNewStyle::ThreadNewStyle() : state_{std::make_unique<ThreadNewStyleState_Idle>()} {}

//
// ThreadNewStyle2
//
// @@@ sample begin 1:0

void ThreadNewStyle2::run_idle()
{
    // スレッドの始動処理
    // @@@ ignore begin
    // @@@ ignore end

    // ステートの切り替え
    run_       = &ThreadNewStyle2::run_running;
    suspend_   = &ThreadNewStyle2::suspend_running;
    state_str_ = state_str_running_;
}

void ThreadNewStyle2::abort_running()
{
    // スレッドのアボート処理
    // @@@ ignore begin
    // @@@ ignore end

    // ステートの切り替え
    run_       = &ThreadNewStyle2::run_idle;
    suspend_   = &ThreadNewStyle2::suspend_idle;
    state_str_ = state_str_idle_;
}

void ThreadNewStyle2::suspend_running()
{
    // スレッドのサスペンド処理
    // @@@ ignore begin
    // @@@ ignore end

    // ステートの切り替え
    run_       = &ThreadNewStyle2::run_suspending;
    suspend_   = &ThreadNewStyle2::suspend_suspending;
    state_str_ = state_str_suspending_;
}

void ThreadNewStyle2::run_suspending()
{
    // スレッドのレジューム処理
    // @@@ ignore begin
    // @@@ ignore end

    // ステートの切り替え
    run_       = &ThreadNewStyle2::run_running;
    suspend_   = &ThreadNewStyle2::suspend_running;
    state_str_ = state_str_running_;
}
// @@@ sample end

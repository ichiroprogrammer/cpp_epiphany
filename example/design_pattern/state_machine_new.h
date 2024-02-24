#pragma once

#include <memory>
#include <string>

// @@@ sample begin 0:0

/// @class ThreadNewStyleState
/// @brief ThreadNewStyleのステートを表す基底クラス
class ThreadNewStyleState {
public:
    ThreadNewStyleState()          = default;
    virtual ~ThreadNewStyleState() = default;

    std::unique_ptr<ThreadNewStyleState> Abort()  // NVI
    {
        return abort_thread();
    }

    std::unique_ptr<ThreadNewStyleState> Run()  // NVI
    {
        return run_thread();
    }

    std::unique_ptr<ThreadNewStyleState> Suspend()  // NVI
    {
        return suspend_thread();
    }

    std::string_view GetStateStr() const noexcept { return get_state_str(); }

private:
    virtual std::unique_ptr<ThreadNewStyleState> abort_thread()
    {
        return {};  // デフォルトでは何もしない。
    }

    virtual std::unique_ptr<ThreadNewStyleState> run_thread()
    {
        return {};  // デフォルトでは何もしない。
    }

    virtual std::unique_ptr<ThreadNewStyleState> suspend_thread()
    {
        return {};  // デフォルトでは何もしない。
    }

    virtual std::string_view get_state_str() const noexcept = 0;
};
// @@@ sample end

// @@@ sample begin 0:1

class ThreadNewStyle final {
public:
    ThreadNewStyle();

    void Abort() { change_state(state_->Abort()); }

    void Run() { change_state(state_->Run()); }

    void Suspend() { change_state(state_->Suspend()); }

    std::string_view GetStateStr() const noexcept { return state_->GetStateStr(); }

private:
    std::unique_ptr<ThreadNewStyleState> state_;

    void change_state(std::unique_ptr<ThreadNewStyleState>&& new_state) noexcept
    {
        if (new_state) {
            state_ = std::move(new_state);
        }
    }
};
// @@@ sample end
// @@@ sample begin 1:0

class ThreadNewStyle2 final {
public:
    ThreadNewStyle2() noexcept {}

    void             Abort() { (this->*abort_)(); }
    void             Run() { (this->*run_)(); }
    void             Suspend() { (this->*suspend_)(); }
    std::string_view GetStateStr() const noexcept { return state_str_; }

private:
    void (ThreadNewStyle2::*abort_)()   = &ThreadNewStyle2::abort_idle;
    void (ThreadNewStyle2::*run_)()     = &ThreadNewStyle2::run_idle;
    void (ThreadNewStyle2::*suspend_)() = &ThreadNewStyle2::suspend_idle;
    std::string_view state_str_{state_str_idle_};

    void                                 abort_idle() {}  // do nothing
    void                                 run_idle();
    void                                 suspend_idle() {}  // do nothing
    static inline std::string_view const state_str_idle_{"Idle"};

    void                                 abort_running();
    void                                 run_running() {}  // do nothing
    void                                 suspend_running();
    static inline std::string_view const state_str_running_{"Running"};

    void                                 abort_suspending();
    void                                 run_suspending();
    void                                 suspend_suspending() {}  // do nothing
    static inline std::string_view const state_str_suspending_{"Suspending"};
};
// @@@ sample end

#pragma once

#include <cstdint>
#include <list>

class Observer;

// @@@ sample begin 0:0

/// @class SubjectOK
/// @brief 監視されるクラス。SetNumでの状態変更をObserverOK_Nに通知する。
///        Observerパターンの使用例。
class SubjectOK final {
public:
    SubjectOK() : observers_{}, num_{0} {}

    void SetNum(uint32_t num)
    {
        if (num_ == num) {
            return;
        }

        num_ = num;

        notify();  // subjectが変更されたことをobserverへ通知
    }

    void     Attach(Observer& observer);           // Observerの登録
    void     Detach(Observer& observer) noexcept;  // Observerの登録解除
    uint32_t GetNum() const noexcept { return num_; }

private:
    void notify() const;

    std::list<Observer*> observers_;
    // @@@ ignore begin
    uint32_t num_;

    SubjectOK(SubjectOK const&)            = delete;
    SubjectOK& operator=(SubjectOK const&) = delete;
    SubjectOK(SubjectOK&&)                 = delete;
    SubjectOK& operator=(SubjectOK&&)      = delete;
    // @@@ ignore end
};

/// @class Observer
/// @brief SubjectOKを監視するクラスの基底クラス
class Observer {
public:
    Observer() = default;
    void Update(SubjectOK const& subject) { update(subject); }

    // @@@ ignore begin
    virtual ~Observer() = default;

    // @@@ ignore end
private:
    virtual void update(SubjectOK const& subject) = 0;
    // @@@ ignore begin

    Observer(Observer const&)            = delete;
    Observer& operator=(Observer const&) = delete;
    Observer(Observer&&)                 = delete;
    Observer& operator=(Observer&&)      = delete;
    // @@@ ignore end
};
// @@@ sample end

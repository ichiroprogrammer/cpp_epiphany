#pragma once

#include <cstdint>

class ObserverNG_0;
class ObserverNG_1;
class ObserverNG_2;

// @@@ sample begin 0:0

/// @class SubjectNG
/// @brief 監視されるクラス。SetNumでの状態変更をObserverNG_Nに通知する。
///        Observerパターンを使用しない例。
class SubjectNG final {
public:
    explicit SubjectNG(ObserverNG_0& ng_0, ObserverNG_1& ng_1, ObserverNG_2& ng_2) noexcept
        : num_{0}, ng_0_{ng_0}, ng_1_{ng_1}, ng_2_{ng_2}
    {
    }

    void SetNum(uint32_t num);
    // @@@ ignore begin
    uint32_t GetNum() const noexcept { return num_; }

private:
    void notify();

    uint32_t      num_;
    ObserverNG_0& ng_0_;
    ObserverNG_1& ng_1_;
    ObserverNG_2& ng_2_;
    // @@@ ignore end
};
// @@@ sample end

#pragma once
#include "suppress_warning.h"

class SubjectNG;

// @@@ sample begin 0:0

/// @class ObserberNG_N
/// @brief SubjectNGからの変更通知をUpdate()で受け取る。
///        Observerパターンを使用しない例。
class ObserverNG_0 {
public:
    ObserverNG_0() = default;

    virtual void Update(SubjectNG const& subject)  // テストのためにvirtual
    {
        // 何らかの処理
        IGNORE_UNUSED_VAR(subject);  // @@@ delete
    }

    virtual ~ObserverNG_0() = default;
    // 何らかの定義、宣言
};

class ObserverNG_1 {
public:
    // @@@ ignore begin
    ObserverNG_1() = default;

    void Update(SubjectNG const& subject);
    // @@@ ignore end
};

class ObserverNG_2 {
public:
    // @@@ ignore begin
    ObserverNG_2() = default;

    void Update(SubjectNG const& subject);
    // @@@ ignore end
};
// @@@ sample end

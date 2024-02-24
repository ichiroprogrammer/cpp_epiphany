#include "subject_ok.h"

// @@@ sample begin 0:0

/// @class ObserverOK_0
/// @brief SubjectOKからの変更通知をUpdate()で受け取る。
///        Observerパターンの使用例。
class ObserverOK_0 : public Observer {
    // @@@ ignore begin
public:
    ObserverOK_0() = default;

    // @@@ ignore end
private:
    virtual void update(SubjectOK const& subject) override;
};

class ObserverOK_1 : public Observer {
    // @@@ ignore begin
public:
    ObserverOK_1() = default;

    // @@@ ignore end
private:
    virtual void update(SubjectOK const& subject) override;
};

class ObserverOK_2 : public Observer {
    // @@@ ignore begin
public:
    ObserverOK_2() = default;

    // @@@ ignore end
private:
    virtual void update(SubjectOK const& subject) override;
};
// @@@ sample end

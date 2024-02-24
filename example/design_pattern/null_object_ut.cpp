#include <memory>

#include "gtest_wrapper.h"

namespace {
namespace NotUseNullObject {
// @@@ sample begin 0:0

class A {
public:
    // @@@ ignore begin
    A()          = default;
    virtual ~A() = default;

    // @@@ ignore end
    bool Action() noexcept
    {
        // do something
        // @@@ ignore begin
        auto result = true;

        // @@@ ignore end
        return result;
    }
    // @@@ ignore begin
    // @@@ ignore end
};

bool ActionOldStyle(A* a) noexcept
{
    if (a != nullptr) {  // ←このif文を消すためのパターン。
        return a->Action();
    }
    else {
        return false;
    }
}
// @@@ sample end
}  // namespace NotUseNullObject
namespace UseNullObject {
// @@@ sample begin 0:1

class A {
public:
    // @@@ ignore begin
    A()          = default;
    virtual ~A() = default;

    // @@@ ignore end
    bool Action() noexcept { return action(); }

private:
    virtual bool action() noexcept
    {
        // do something
        // @@@ ignore begin
        auto result = true;

        // @@@ ignore end
        return result;
    }
    // @@@ ignore begin
    // @@@ ignore end
};

class ANull final : public A {
    // @@@ ignore begin
public:
    ANull()                   = default;
    virtual ~ANull() override = default;

    // @@@ ignore end
private:
    virtual bool action() noexcept override { return false; }
};

bool ActionNewStyle(A& a) noexcept
{
    return a.Action();  // ←Null Objectによりif文が消えた。
}
// @@@ sample end
}  // namespace UseNullObject

TEST(NullObject, just_call)
{
    auto a = NotUseNullObject::A{};

    NotUseNullObject::ActionOldStyle(&a);
    NotUseNullObject::ActionOldStyle(nullptr);

    auto b = UseNullObject::A{};
    UseNullObject::ActionNewStyle(b);

    auto b_null = UseNullObject::ANull{};
    UseNullObject::ActionNewStyle(b_null);
}
}  // namespace

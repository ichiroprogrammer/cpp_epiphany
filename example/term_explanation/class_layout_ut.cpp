#include "gtest_wrapper.h"

// 以下はあえてグローバル
// @@@ sample begin 0:0

class X {
public:
    virtual int64_t GetX() { return x_; }
    virtual ~X() {}

private:
    int64_t x_{1};
};

class Y : public X {
public:
    virtual int64_t GetX() override { return X::GetX() + y_; }
    virtual int64_t GetY() { return y_; }
    virtual ~Y() override {}

private:
    int64_t y_{2};
};

class Z : public Y {
public:
    virtual int64_t GetX() override { return Y::GetX() + z_; }
    virtual int64_t GetY() override { return Y::GetY() + z_; }
    virtual int64_t GetZ() { return z_; }
    virtual ~Z() override {}

private:
    int64_t z_{3};
};
// @@@ sample end

namespace ClassLayout {
void f()
{
    // @@@ sample begin 0:1

    auto z_ptr = new Z;
    // @@@ sample end

    delete z_ptr;
}
}  // namespace ClassLayout

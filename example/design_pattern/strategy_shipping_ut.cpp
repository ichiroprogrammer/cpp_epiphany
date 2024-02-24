#include "gtest_wrapper.h"

namespace Strategy {
namespace NG {
#define SHIP_TO_JAPAN 1
#define SHIP_TO_US 2
#define SHIP_TO_EU 3

#define SHIPPING SHIP_TO_JAPAN

// @@@ sample begin 0:0

class X {
public:
    X() = default;

    int32_t DoSomething()
    {
        int32_t ret{0};

#if SHIPPING == SHIP_TO_JAPAN
        // 日本向けの何らかの処理
        ret = 1;  // @@@ delete
#elif SHIPPING == SHIP_TO_US
        // US向けの何らかの処理
        ret = 2;  // @@@ delete
#elif SHIPPING == SHIP_TO_JAPAN
        // EU向けの何らかの処理
        ret = 3;  // @@@ delete
#else
#error "SHIPPING must be defined"
#endif
        return ret;
    }

private:
    // 何らかの宣言
};
// @@@ sample end

TEST(Strategy, shipping_ng)
{
    // @@@ sample begin 0:1

    X x;

    x.DoSomething();
    // @@@ sample end

    ASSERT_EQ(1, x.DoSomething());
}
}  // namespace NG

namespace OK_0 {

// @@@ sample begin 1:0

class ShippingOp {
public:
    virtual int32_t DoSomething() = 0;
    virtual ~ShippingOp()         = default;
};

class X {
public:
    X() = default;

    int32_t DoSomething(ShippingOp& shipping)
    {
        int32_t ret = shipping.DoSomething();

        // 何らかの処理

        return ret;
    }

private:
    // 何らかの宣言
};
// @@@ sample end
// @@@ sample begin 1:1

class ShippingOp_Japan : public ShippingOp {
public:
    ShippingOp_Japan();
    int32_t DoSomething() override;
    ~ShippingOp_Japan() override;

private:
    // 何らかの宣言
};
// @@@ sample end

ShippingOp_Japan::ShippingOp_Japan() {}
int32_t ShippingOp_Japan::DoSomething() { return 1; }
ShippingOp_Japan::~ShippingOp_Japan() {}

TEST(Strategy, shipping_ok_0)
{
    // @@@ sample begin 1:2

    X                x;
    ShippingOp_Japan sj;

    x.DoSomething(sj);
    // @@@ sample end
    ASSERT_EQ(1, x.DoSomething(sj));
}
}  // namespace OK_0
namespace OK_1 {

// @@@ sample begin 2:0

class ShippingOp {
public:
    virtual int32_t DoSomething() = 0;
    virtual ~ShippingOp()         = default;
};

class X {
public:
    explicit X(std::unique_ptr<ShippingOp> shipping) : shipping_{std::move(shipping)} {}

    int32_t DoSomething()
    {
        int32_t ret = shipping_->DoSomething();

        // 何らかの処理

        return ret;
    }

private:
    std::unique_ptr<ShippingOp> shipping_;
    // 何らかの宣言
};
// @@@ sample end
// @@@ sample begin 2:1

class ShippingOp_Japan : public ShippingOp {
public:
    ShippingOp_Japan();
    int32_t DoSomething() override;
    ~ShippingOp_Japan() override;

private:
    // 何らかの宣言
};
// @@@ sample end

ShippingOp_Japan::ShippingOp_Japan() {}
int32_t ShippingOp_Japan::DoSomething() { return 1; }
ShippingOp_Japan::~ShippingOp_Japan() {}

TEST(Strategy, shipping_ok_1)
{
    // @@@ sample begin 2:2

    X x{std::unique_ptr<ShippingOp>(new ShippingOp_Japan)};

    x.DoSomething();
    // @@@ sample end
    ASSERT_EQ(1, x.DoSomething());
}
}  // namespace OK_1
}  // namespace Strategy

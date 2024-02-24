#include "gtest_wrapper.h"

#include "factory_shipping.h"

namespace Factory {

// @@@ sample begin 0:0

class ShippingOp_Japan : public ShippingOp {
public:
    ShippingOp_Japan();
    int32_t DoSomething() override;
    ~ShippingOp_Japan() override;

private:
    // 何らかの宣言
};
// @@@ sample end

class ShippingOp_US : public ShippingOp {
public:
    ShippingOp_US();
    int32_t DoSomething() override;
    ~ShippingOp_US() override;

private:
    // 何らかの宣言
};

class ShippingOp_EU : public ShippingOp {
public:
    ShippingOp_EU();
    int32_t DoSomething() override;
    ~ShippingOp_EU() override;

private:
    // 何らかの宣言
};

ShippingOp_Japan::ShippingOp_Japan() {}
int32_t ShippingOp_Japan::DoSomething() { return 1; }
ShippingOp_Japan::~ShippingOp_Japan() {}

ShippingOp_US::ShippingOp_US() {}
int32_t ShippingOp_US::DoSomething() { return 2; }
ShippingOp_US::~ShippingOp_US() {}

ShippingOp_EU::ShippingOp_EU() {}
int32_t ShippingOp_EU::DoSomething() { return 3; }
ShippingOp_EU::~ShippingOp_EU() {}

namespace NG {
TEST(Factory, shipping_ng)
{
#define SHIPPING SHIP_TO_JAPAN

    // clang-format off
// @@@ sample begin 1:0

// SHIPPINGはmake等のビルドツールから渡される

#if SHIPPING == SHIP_TO_JAPAN
    auto shipping = ShippingOp_Japan{};
#elif SHIPPING == SHIP_TO_US
    auto shipping = ShippingOp_US{};
#elif SHIPPING == SHIP_TO_EU
    auto shipping = ShippingOp_EU{};
#else
#error "SHIPPING must be defined"
#endif

    shipping.DoSomething();
    // @@@ sample end
    // clang-format on
    ASSERT_EQ(1, shipping.DoSomething());
}
}  // namespace NG

// @@@ sample begin 2:0

template <>
std::unique_ptr<ShippingOp> ShippingOpFactory<ShippingRegion::Japan>()
{
    return std::unique_ptr<ShippingOp>{new ShippingOp_Japan};
}
// @@@ sample end

template <>
std::unique_ptr<ShippingOp> ShippingOpFactory<ShippingRegion::US>()
{
    return std::unique_ptr<ShippingOp>{new ShippingOp_US};
}

template <>
std::unique_ptr<ShippingOp> ShippingOpFactory<ShippingRegion::EU>()
{
    return std::unique_ptr<ShippingOp>{new ShippingOp_EU};
}

// @@@ sample begin 3:0

std::unique_ptr<ShippingOp> ShippingOpFactory(ShippingRegionType_Japan)
{
    return std::unique_ptr<ShippingOp>{new ShippingOp_Japan};
}
// @@@ sample end

std::unique_ptr<ShippingOp> ShippingOpFactory(ShippingRegionType_US)
{
    return std::unique_ptr<ShippingOp>{new ShippingOp_US};
}

std::unique_ptr<ShippingOp> ShippingOpFactory(ShippingRegionType_EU)
{
    return std::unique_ptr<ShippingOp>{new ShippingOp_EU};
}
}  // namespace Factory

#include "gtest_wrapper.h"

#include "factory_shipping.h"
#include "suppress_warning.h"

namespace Factory {

namespace OK {

TEST(Factory, shipping_ok_0)
{
#undef SHIPPING
#define SHIPPING ShippingRegion::Japan
    // @@@ sample begin 1:1

    // SHIPPINGはmake等のビルドツールからShippingRegionのいづれかとして渡される
    auto shipping = ShippingOpFactory<SHIPPING>();

    shipping->DoSomething();
    // @@@ sample end
    ASSERT_EQ(1, shipping->DoSomething());

    auto shipping_j  = ShippingOpFactory<ShippingRegion::Japan>();
    auto shipping_us = ShippingOpFactory<ShippingRegion::US>();
    auto shipping_eu = ShippingOpFactory<ShippingRegion::EU>();

    ASSERT_EQ(1, shipping_j->DoSomething());
    ASSERT_EQ(2, shipping_us->DoSomething());
    ASSERT_EQ(3, shipping_eu->DoSomething());
}

TEST(Factory, shipping_ok_1)
{
#undef SHIPPING
#define SHIPPING ShippingRegion::US
    // @@@ sample begin 1:2

    // SHIPPINGはmake等のビルドツールからShippingRegionのいづれかとして渡される
    auto shipping = ShippingOpFactory(ShippingRegion2Type<SHIPPING>{});

    shipping->DoSomething();
    // @@@ sample end
    ASSERT_EQ(2, shipping->DoSomething());

    auto shipping_j  = ShippingOpFactory(ShippingRegion2Type<ShippingRegion::Japan>{});
    auto shipping_us = ShippingOpFactory(ShippingRegion2Type<ShippingRegion::US>{});
    auto shipping_eu = ShippingOpFactory(ShippingRegion2Type<ShippingRegion::EU>{});

    ASSERT_EQ(1, shipping_j->DoSomething());
    ASSERT_EQ(2, shipping_us->DoSomething());
    ASSERT_EQ(3, shipping_eu->DoSomething());
}
}  // namespace OK
}  // namespace Factory

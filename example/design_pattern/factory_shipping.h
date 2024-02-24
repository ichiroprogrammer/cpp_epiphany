#pragma once
#include <memory>
#include <type_traits>

namespace Factory {
// @@@ sample begin 0:0

#define SHIP_TO_JAPAN 1
#define SHIP_TO_US 2
#define SHIP_TO_EU 3

class ShippingOp {
public:
    virtual int32_t DoSomething() = 0;
    virtual ~ShippingOp()         = default;
};
// @@@ sample end
// @@@ sample begin 1:0

// ShippingOpクラスは改善前のコードと同じ

enum class ShippingRegion { Japan, US, EU };
// @@@ sample end
// @@@ sample begin 1:1

template <ShippingRegion>
std::unique_ptr<ShippingOp> ShippingOpFactory();  // ShippingOpFactory特殊化のための宣言

template <>
std::unique_ptr<ShippingOp> ShippingOpFactory<ShippingRegion::Japan>();  // 特殊化関数の宣言

template <>
std::unique_ptr<ShippingOp> ShippingOpFactory<ShippingRegion::US>();  // 特殊化関数の宣言

template <>
std::unique_ptr<ShippingOp> ShippingOpFactory<ShippingRegion::EU>();  // 特殊化関数の宣言
// @@@ sample end

// @@@ sample begin 2:0

template <ShippingRegion R>
class ShippingRegion2Type : std::integral_constant<ShippingRegion, R> {
};

using ShippingRegionType_Japan = ShippingRegion2Type<ShippingRegion::Japan>;
using ShippingRegionType_US    = ShippingRegion2Type<ShippingRegion::US>;
using ShippingRegionType_EU    = ShippingRegion2Type<ShippingRegion::EU>;

std::unique_ptr<ShippingOp> ShippingOpFactory(ShippingRegionType_Japan);
std::unique_ptr<ShippingOp> ShippingOpFactory(ShippingRegionType_US);
std::unique_ptr<ShippingOp> ShippingOpFactory(ShippingRegionType_EU);
// @@@ sample end
}  // namespace Factory

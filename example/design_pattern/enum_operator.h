#include <cstdint>

#pragma once

namespace NotUseBitmaskType {
// @@@ sample begin 0:0

class Animal {
public:
    struct PhisicalAbility {  // オブジェクトの状態を表すためのビットマスク
        static constexpr auto Run  = 0b0001U;
        static constexpr auto Fly  = 0b0010U;
        static constexpr auto Swim = 0b0100U;
    };

    // paにはPhisicalAbilityのみを受け入れたいが、実際にはすべてのuint32_tを受け入れる。
    explicit Animal(uint32_t pa) noexcept : phisical_ability_{pa} {}

    uint32_t GetPhisicalAbility() const noexcept { return phisical_ability_; }
    // @@@ ignore begin

private:
    uint32_t const phisical_ability_;
    // @@@ ignore end
};
// @@@ sample end
}  // namespace NotUseBitmaskType

namespace UseBitmaskType {
// @@@ sample begin 1:0

class Animal {
public:
    enum class PhisicalAbility : uint32_t {
        Run  = 0b0001,
        Fly  = 0b0010,
        Swim = 0b0100,
    };

    explicit Animal(PhisicalAbility pa) noexcept : phisical_ability_{pa} {}

    PhisicalAbility GetPhisicalAbility() const noexcept { return phisical_ability_; }

private:
    PhisicalAbility const phisical_ability_;
};

// &, | &=, |=, IsTrue, IsFalseの定義
constexpr Animal::PhisicalAbility operator&(Animal::PhisicalAbility x,
                                            Animal::PhisicalAbility y) noexcept
{
    return static_cast<Animal::PhisicalAbility>(static_cast<uint32_t>(x)
                                                & static_cast<uint32_t>(y));
}

constexpr Animal::PhisicalAbility operator|(Animal::PhisicalAbility x,
                                            Animal::PhisicalAbility y) noexcept
{
    return static_cast<Animal::PhisicalAbility>(static_cast<uint32_t>(x)
                                                | static_cast<uint32_t>(y));
}

inline Animal::PhisicalAbility& operator&=(Animal::PhisicalAbility& x,
                                           Animal::PhisicalAbility  y) noexcept
{
    return x = x & y;
}

// @@@ ignore begin

inline Animal::PhisicalAbility& operator|=(Animal::PhisicalAbility& x,
                                           Animal::PhisicalAbility  y) noexcept
{
    return x = x | y;
}

inline bool IsTrue(Animal::PhisicalAbility x) noexcept { return static_cast<bool>(x); }

inline bool IsFalse(Animal::PhisicalAbility x) noexcept { return !IsTrue(x); }
// @@@ ignore end
// @@@ sample end
// @@@ sample begin 2:0

class Mammals : public Animal {  // 哺乳類
public:
    static Mammals& Human() noexcept
    {
        static auto inst = Mammals{PhisicalAbility::Run | PhisicalAbility::Swim};
        return inst;
    }

    static Mammals& Bat() noexcept
    {
        static auto inst = Mammals{PhisicalAbility::Run | PhisicalAbility::Fly};
        return inst;
    }

    static Mammals& Whale() noexcept
    {
        static auto inst = Mammals{PhisicalAbility::Swim};
        return inst;
    }

    bool Act();

private:
    Mammals(PhisicalAbility pa) noexcept : Animal{pa} {}
};
// @@@ sample end
}  // namespace UseBitmaskType

#include <cstddef>
#include <type_traits>

#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace {
TEST(ArithmeticConversion, arithmetic_conversion)
{
    // @@@ sample begin 0:0

    bool           bval{};
    char           cval{};
    short          sval{};
    unsigned short usval{};
    int            ival{};
    unsigned int   uival{};
    long           lval{};
    unsigned long  ulval{};
    float          fval{};
    double         dval{};

    auto ret_0 = 3.14159 + 'a';  // 'a'は汎整数拡張でintになった後、さらに通常算術変換でdoubleに
    static_assert(std::is_same<decltype(ret_0), double>::value, "");

    auto ret_1 = dval + ival;  // ivalは通常算術変換でdoubleに
    static_assert(std::is_same<decltype(ret_1), double>::value, "");

    auto ret_2 = dval + fval;  // fvalは通常算術変換でdoubleに
    static_assert(std::is_same<decltype(ret_2), double>::value, "");

    auto ret_3 = ival = dval;  // dvalは通常算術変換でintに
    static_assert(std::is_same<decltype(ret_3), int>::value, "");

    bval = dval;  // dvalは通常算術変換でboolに
    ASSERT_FALSE(bval);

    auto ret_4 = cval + fval;  // cvalは汎整数拡張でintになった後、さらに通常算術変換でfloatに
    static_assert(std::is_same<decltype(ret_4), float>::value, "");

    auto ret_5 = sval + cval;  // svalとcvalは汎整数拡張でintに
    static_assert(std::is_same<decltype(ret_5), int>::value, "");

    auto ret_6 = cval + lval;  // cvalはは汎整数拡張でintになった後、通常算術変換でlongに
    static_assert(std::is_same<decltype(ret_6), long>::value, "");

    auto ret_7 = ival + ulval;  // ivalは通常算術変換でunsigned longに
    static_assert(std::is_same<decltype(ret_7), unsigned long>::value, "");

    auto ret_8 = usval + ival;  // usvalは汎整数拡張でintに
                                // ただし、この変換はunsigned shortとintのサイズに依存する
    static_assert(std::is_same<decltype(ret_8), int>::value, "");

    auto ret_9 = uival + lval;  // uivalは通常算術変換でlongに
                                // ただし、この変換はunsigned intとlongのサイズに依存する
    static_assert(std::is_same<decltype(ret_9), long>::value, "");
    // @@@ sample end
    IGNORE_UNUSED_VAR(ret_0, ret_1, ret_2, ret_3, ret_4, ret_5, ret_6, ret_7, ret_8, ret_9);

    {
        // @@@ sample begin 0:1

        int i{-1};
        // int8_t i8 {i};  縮小型変換によりコンパイル不可
        int8_t i8 = i;  // intからint8_tへの型変換
        // これには問題ないが

        ASSERT_EQ(-1, i8);

        // uint8_t ui8 {i};  縮小型変換によりコンパイル不可
        uint8_t ui8 = i;  // intからuint8_tへの型変換
        // おそらく意図通りではない

        ASSERT_EQ(255, ui8);
        // @@@ sample end
    }
    {
        SUPPRESS_WARN_BEGIN;
        SUPPRESS_WARN_SIGN_COMPARE;
        // @@@ sample begin 0:2

        int          i{-1};
        unsigned int ui{1};

        // ASSERT_TRUE(i < ui);
        ASSERT_TRUE(i > ui);  // 算術変換の影響で、-1 < 1が成立しない

        signed short   s{-1};
        unsigned short us{1};

        ASSERT_TRUE(s < us);  // 汎整数拡張により、-1 < 1が成立
        // @@@ sample end
        SUPPRESS_WARN_END;
    }
}

TEST(ArithmeticConversion, integral_promotion)
{
    // @@@ sample begin 1:0

    bool bval;
    static_assert(std::is_same<int, decltype(bval + bval)>::value, "");

    char cval;
    static_assert(std::is_same<int, decltype(cval + cval)>::value, "");

    unsigned char ucval = 128;
    static_assert(std::is_same<int, decltype(ucval + ucval)>::value, "");
    ASSERT_EQ(256, ucval + ucval);  // 汎整数拡張により256になる

    static_assert(std::is_same<int, decltype(cval + ucval)>::value, "");

    short sval;
    static_assert(std::is_same<int, decltype(sval + sval)>::value, "");

    unsigned short usval;
    static_assert(std::is_same<int, decltype(usval + usval)>::value, "");

    static_assert(std::is_same<int, decltype(sval + usval)>::value, "");
    // @@@ sample end
}
}  // namespace

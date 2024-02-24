#include <algorithm>
#include <type_traits>

#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace {
// @@@ sample begin 0:0

// 従来のenum
enum NormalEnum {
    // @@@ ignore begin
    // @@@ ignore end
};

// enum underlying typeがint8_tに指定された従来のenum
enum NormalEnumWithUnderlyingType : int8_t {
    // @@@ ignore begin
    // @@@ ignore end
};

// enum class
enum class EnumClass {
    // @@@ ignore begin
    // @@@ ignore end
};

// enum underlying typeがint64_tに指定されたenum class
enum class EnumClassWithUnderlyingType : int64_t {
    // @@@ ignore begin
    // @@@ ignore end
};
// @@@ sample end

TEST(TermExplanationMisra, enum_size)
{
    // @@@ sample begin 0:1

    ASSERT_EQ(4, sizeof(NormalEnum));  // 列挙子の値を表現するのに十分なサイズの整数型で処理系依存

    // NormalEnumWithUnderlyingTypeのunderlying typeはint8_t
    static_assert(std::is_same_v<int8_t, std::underlying_type_t<NormalEnumWithUnderlyingType>>);
    ASSERT_EQ(sizeof(int8_t), sizeof(NormalEnumWithUnderlyingType));

    ASSERT_EQ(4, sizeof(EnumClass));  // 列挙子の値を表現するのに十分なサイズの整数型で処理系依存

    // EnumClassWithUnderlyingTypeのunderlying typeはint64_t
    static_assert(std::is_same_v<int64_t, std::underlying_type_t<EnumClassWithUnderlyingType>>);
    ASSERT_EQ(sizeof(int64_t), sizeof(EnumClassWithUnderlyingType));
    // @@@ sample end
}
}  // namespace

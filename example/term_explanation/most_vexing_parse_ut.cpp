#include "gtest_wrapper.h"

#include "nstd_type2str.h"
#include "suppress_warning.h"

// @@@ sample begin 0:0

class Vexing {
public:
    Vexing(int) {}
};
// @@@ sample end

TEST(TermExplanationMisra, most_vexing_parse)
{
    SUPPRESS_WARN_BEGIN;
    SUPPRESS_WARN_CONVERSION_VEXING_PARSE;

    // @@@ sample begin 0:1

    Vexing obj1();        // はローカルオブジェクトobj1の宣言ではない
    Vexing obj2(Vexing);  // ローカルオブジェクトobj2の宣言ではない

    ASSERT_EQ("Vexing ()", Nstd::Type2Str<decltype(obj1)>());
    ASSERT_EQ("Vexing (Vexing)", Nstd::Type2Str<decltype(obj2)>());
    // 上記単体テストが示すように、
    // * obj1はVexingを返す関数
    // * obj2はVexingを引数に取りVexingを返す関数
    // となる。
    // @@@ sample end
    SUPPRESS_WARN_END;
}

#include <iostream>

#include "gtest_wrapper.h"

#include "crtp.h"

namespace CRTP {
// @@@ sample begin 0:0

template <typename T>
class Base {
    // @@@ ignore begin
    // @@@ ignore end
};

class Derived : public Base<Derived> {
    // @@@ ignore begin
    // @@@ ignore end
};
// @@@ sample end

namespace {

TEST(CRTP, testable_visitor)
{
    auto ostring = std::ostringstream{};

    // 出力をキャプチャするため、std::coutに代えてostringを使う
    auto visitor1 = TestablePathnamePrinter1{ostring};
    auto visitor2 = TestablePathnamePrinter2{ostring};

    auto file = File{"visitor.cpp"};
    {
        file.Accept(visitor1);
        auto ret = std::string{ostring.str()};
        ASSERT_EQ("visitor.cpp", ret);
        ostring = std::ostringstream{};
    }
    {
        file.Accept(visitor2);
        auto ret = std::string{ostring.str()};
        ASSERT_EQ("visitor.cpp", ret);
        ostring = std::ostringstream{};
    }

    auto dir = Dir{"find_files_ut_dir/dir0"};
    {
        dir.Accept(visitor1);
        auto ret = std::string{ostring.str()};
        ASSERT_EQ("find_files_ut_dir/dir0/", ret);
        ostring = std::ostringstream{};
    }
    {
        dir.Accept(visitor2);
        auto ret = std::string{ostring.str()};
        ASSERT_EQ("find_files_ut_dir/dir0/file2,find_files_ut_dir/dir0/gile3", ret);
        ostring = std::ostringstream{};
    }

    auto other = OtherEntity{"."};
    {
        other.Accept(visitor1);
        auto ret = std::string{ostring.str()};
        ASSERT_EQ(".(o1)", ret);
        ostring = std::ostringstream{};
    }
    {
        other.Accept(visitor2);
        auto ret = std::string{ostring.str()};
        ASSERT_EQ(".(o2)", ret);
        ostring = std::ostringstream{};
    }
}
}  // namespace
}  // namespace CRTP

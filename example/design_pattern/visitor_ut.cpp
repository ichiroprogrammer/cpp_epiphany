#include <iostream>

#include "gtest_wrapper.h"

#include "visitor.h"

namespace VisitorPattern {
namespace {

TEST(Visitor, visitor)
{
    auto visitor = PathnamePrinter1{};

    auto file  = File{"visitor.cpp"};
    auto dir   = Dir{"find_files_ut_dir"};
    auto other = OtherEntity{"."};

    file.Accept(visitor);
    std::cout << std::endl;

    dir.Accept(visitor);
    std::cout << std::endl;

    other.Accept(visitor);
    std::cout << std::endl;
}

// @@@ sample begin 0:0

TEST(Visitor, testable_visitor)
{
    auto oss = std::ostringstream{};

    // 出力をキャプチャするため、std::coutに代えてossを使う
    auto visitor1 = TestablePathnamePrinter1{oss};
    auto visitor2 = TestablePathnamePrinter2{oss};

    auto file = File{"visitor.cpp"};
    {
        file.Accept(visitor1);
        ASSERT_EQ("visitor.cpp", oss.str());
        oss = {};
    }
    {
        file.Accept(visitor2);
        ASSERT_EQ("visitor.cpp", oss.str());
        oss = {};
    }

    auto dir = Dir{"find_files_ut_dir/dir0"};
    {
        dir.Accept(visitor1);
        ASSERT_EQ("find_files_ut_dir/dir0/", oss.str());
        oss = {};
    }
    {
        dir.Accept(visitor2);
        ASSERT_EQ("find_files_ut_dir/dir0/file2,find_files_ut_dir/dir0/gile3", oss.str());
        oss = {};
    }
}
// @@@ sample end
}  // namespace
}  // namespace VisitorPattern

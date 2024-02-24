#include "gtest_wrapper.h"

#include "nstd_exception.h"

namespace {
// @@@ sample begin 0:0

class Exception : std::exception {
public:
    Exception(char const* filename, uint32_t line_num, char const* msg)
        : what_str_{std::string{filename} + ":" + std::to_string(line_num) + ":" + msg}
    {
    }

    char const* what() const noexcept override { return what_str_.c_str(); }

private:
    std::string what_str_;
};

int32_t div(int32_t a, int32_t b)
{
    if (b == 0) {
        throw Exception{__FILE__, __LINE__, "divided by 0"};  // 24行目
    }

    return a / b;
}
// @@@ sample end

TEST(Template, make_exception_ng)
{
    // 以下のテストは行番号が大切
    // @@@ sample begin 0:1

    auto caught = false;
    try {
        div(1, 0);
    }
    catch (Exception const& e) {
        ASSERT_STREQ("nstd_exception_ut.cpp:24:divided by 0", e.what());
        caught = true;
    }
    ASSERT_TRUE(caught);
    // @@@ sample end

    {
        auto line_num = __LINE__ + 1;  // 次の行番号
        auto exp      = Exception{__FILE__, __LINE__, "some error message"};

        auto oss = std::ostringstream{};
        oss << __FILE__ << ":" << line_num << ":some error message";

        ASSERT_EQ(oss.str(), exp.what());
    }
    {
        auto caught   = false;
        auto line_num = __LINE__ + 2;  // 2行下の行番号
        try {
            throw Exception{__FILE__, __LINE__, "some error message"};
        }
        catch (Exception const& e) {
            auto oss = std::ostringstream{};
            oss << __FILE__ << ":" << line_num << ":some error message";

            ASSERT_EQ(oss.str(), e.what());
            caught = true;
        }

        ASSERT_TRUE(caught);
    }
}
}  // namespace

namespace Nstd {
namespace {
TEST(Template, make_exception)
{
    {
        auto line_num = __LINE__ + 1;  // 次の行番号
        auto exp      = MakeException<std::exception, __LINE__>(__FILE__, "some error message");

        auto oss = std::ostringstream{};
        oss << __FILE__ << ":" << line_num << ":some error message";

        ASSERT_EQ(oss.str(), exp.what());
    }
    {
        // @@@ sample begin 1:0

        auto caught   = false;
        auto line_num = __LINE__ + 2;  // 2行下の行番号
        try {
            throw MakeException<std::exception, __LINE__>(__FILE__, "some error message");
        }
        catch (std::exception const& e) {
            auto oss = std::ostringstream{};
            oss << __FILE__ << ":" << line_num << ":some error message";

            ASSERT_EQ(oss.str(), e.what());
            caught = true;
        }

        ASSERT_TRUE(caught);
        // @@@ sample end
    }
}

// @@@ sample begin 2:0

uint32_t line_num_div;  // エクセプション行を指定

int32_t div(int32_t a, int32_t b)
{
    if (b == 0) {
        line_num_div = __LINE__ + 1;  // 次の行番号
        throw MAKE_EXCEPTION(std::exception, "divided by 0");
    }

    return a / b;
}
// @@@ sample end

TEST(Template, make_exception_f)
{
    // @@@ sample begin 2:1

    auto caught = false;

    try {
        div(1, 0);
    }
    catch (std::exception const& e) {  // リファレンスでcatchしなければならない
        auto oss = std::ostringstream{};
        oss << __FILE__ << ":" << line_num_div << ":divided by 0";
        ASSERT_EQ(oss.str(), e.what());
        caught = true;
    }

    ASSERT_TRUE(caught);
    // @@@ sample end
}
}  // namespace
}  // namespace Nstd

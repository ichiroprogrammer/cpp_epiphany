#include "gtest_wrapper.h"

#include "nstd_seq.h"

namespace Nstd {
namespace {
// @@@ sample begin 0:0

template <size_t N>
struct seq_test {
    template <size_t... S>
    constexpr seq_test(index_sequence<S...>) noexcept : data{S...}
    {
    }
    int const data[N];
};
// @@@ sample end

TEST(Template, seq)
{
    static_assert(std::is_base_of_v<index_sequence<0, 1, 2>, make_index_sequence<3>>);
    static_assert(std::is_base_of_v<index_sequence<0, 1, 2, 3, 4, 5>, make_index_sequence<6>>);

    // @@@ sample begin 0:1

    constexpr auto st = seq_test<3>{index_sequence<1, 2, 3>()};
    ASSERT_EQ(1, st.data[0]);
    ASSERT_EQ(2, st.data[1]);
    ASSERT_EQ(3, st.data[2]);
    // @@@ sample end
}

// @@@ sample begin 1:0

template <size_t N>
class seq_test2 {
public:
    template <size_t... S>
    constexpr seq_test2(char const (&str)[N], index_sequence<S...>) noexcept : string_{str[S]...}
    {
    }

    constexpr char const (&String() const noexcept)[N] { return string_; }

private:
    char const string_[N];
};
// @@@ sample end

TEST(Template, seq2)
{
    // @@@ sample begin 1:1

    constexpr char const str[]{"123"};

    constexpr auto st = seq_test2<4>{str, index_sequence<0, 1, 2>()};
    ASSERT_STREQ("123", st.String());

    constexpr auto st2 = seq_test2<4>{str, make_index_sequence<sizeof(str) - 1>()};
    ASSERT_STREQ("123", st2.String());
    // @@@ sample end
}
}  // namespace
}  // namespace Nstd

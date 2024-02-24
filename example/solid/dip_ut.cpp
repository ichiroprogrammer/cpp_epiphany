#include <iostream>

#include "gtest_wrapper.h"

#include "dip_client_ng.h"
#include "dip_client_ok.h"
#include "dip_server_ng.h"
#include "dip_server_ok.h"

namespace {
// @@@ sample begin 0:0

TEST(DIP, ng_pattern)
{
    auto server = ServerNG::Server{};
    auto client = ClientNG::Client{server};

    auto actual = client.GetString(1);
    ASSERT_EQ("hello", actual);

    actual = client.GetString(2);
    ASSERT_EQ("good bye", actual);

    actual = client.GetString(3);
    ASSERT_EQ("unknown", actual);
}
// @@@ sample end
// @@@ sample begin 0:1

// 使用方法は、ServerNG, ClientNGと同じ。
TEST(DIP, ok_pattern)
{
    auto server = ServerOK::Server{};
    auto client = ClientOK::Client{server};

    // 以下、ng_paternと同じ
    // @@@ ignore begin
    auto actual = client.GetString(1);
    ASSERT_EQ("hello", actual);

    actual = client.GetString(2);
    ASSERT_EQ("good bye", actual);

    actual = client.GetString(3);
    ASSERT_EQ("unknown", actual);
    // @@@ ignore end
}
// @@@ sample end
}  // namespace

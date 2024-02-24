#include <unistd.h>

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "gtest_wrapper.h"

#include "bare_server.h"
#include "bare_server_wrapper.h"
#include "wrapped_server.h"
#include "wrapped_server_proxy.h"

namespace {

// @@@ sample begin 0:0

/// @fn bare_client
/// @brief 非同期サービスを隠蔽していないBareServerを使用したときのクライアントの例
std::vector<std::string> bare_client(BareServer& bs)
{
    auto const wfd = bs.GetPipeW();
    auto const rfd = bs.GetPipeR();
    auto       ret = std::vector<std::string>{};

    bs.Start();

    auto packet = Packet{};
    char buffer[30];

    packet.cmd = Cmd::SayHello;
    write(wfd, &packet, sizeof(packet));

    auto read_ret = read(rfd, buffer, sizeof(buffer));
    assert(read_ret > 0);

    ret.emplace_back(buffer);

    packet.cmd = Cmd::SayGoodbye;
    write(wfd, &packet, sizeof(packet));

    read_ret = read(rfd, buffer, sizeof(buffer));
    assert(read_ret > 0);

    ret.emplace_back(buffer);

    packet.cmd = Cmd::Shutdown;
    write(wfd, &packet, sizeof(packet));

    bs.Wait();

    return ret;
}
// @@@ sample end

// @@@ sample begin 0:1

/// @fn bare_wrapper_client
/// @brief BareServerを使いやすくラップしたBareServerWrapperを使用したときのクライアントの例
std::vector<std::string> bare_wrapper_client(BareServerWrapper& bsw)
{
    auto ret = std::vector<std::string>{};

    bsw.Start();

    ret.emplace_back(bsw.SayHello());

    ret.emplace_back(bsw.SayGoodbye());

    bsw.Shutdown();

    return ret;
}
// @@@ sample end

// @@@ sample begin 0:2

/// @fn wrapped_client
/// @brief 非同期サービスを隠蔽しているWrappedServerを使用したときのクライアントの例
std::vector<std::string> wrapped_client(WrappedServer& ws)
{
    auto ret = std::vector<std::string>{};

    ws.Start();

    ret.emplace_back(ws.SayHello());

    ret.emplace_back(ws.SayGoodbye());

    ws.Shutdown();

    return ret;
}
// @@@ sample end

TEST(Proxy, server_client)
{
    auto bs = BareServer{};
    ASSERT_EQ((std::vector<std::string>{"Hello", "Goodbye"}), bare_client(bs));

    auto bsw = BareServerWrapper{};
    ASSERT_EQ((std::vector<std::string>{"Hello", "Goodbye"}), bare_wrapper_client(bsw));

    auto ws = WrappedServer{};
    ASSERT_EQ((std::vector<std::string>{"Hello", "Goodbye"}), wrapped_client(ws));

    auto wsp = WrappedServerProxy{};
    ASSERT_EQ((std::vector<std::string>{"Hello", "Goodbye"}), wrapped_client(wsp));
}
}  // namespace

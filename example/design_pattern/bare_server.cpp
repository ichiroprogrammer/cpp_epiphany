#include <unistd.h>

#include <cassert>
#include <cstdint>
#include <iostream>

#include "bare_server.h"

// @@@ sample begin 0:0

namespace {
bool cmd_dispatch(int wfd, Cmd cmd) noexcept
{
    static char const hello[]   = "Hello";
    static char const goodbye[] = "Goodbye";

    switch (cmd) {
    case Cmd::SayHello:
        write(wfd, hello, sizeof(hello));
        break;
    case Cmd::SayGoodbye:
        write(wfd, goodbye, sizeof(goodbye));
        break;
    case Cmd::Shutdown:
    default:
        std::cout << "Shutdown" << std::endl;
        return false;
    }

    return true;
}

void thread_entry(int rfd, int wfd) noexcept
{
    for (;;) {
        auto packet = Packet{};

        if (read(rfd, &packet, sizeof(packet)) < 0) {
            continue;
        }

        if (!cmd_dispatch(wfd, packet.cmd)) {
            break;
        }
    }
}
}  // namespace

BareServer::BareServer() noexcept : to_server_{-1, -1}, to_client_{-1, -1}, thread_{}
{
    auto ret = pipe(to_server_);
    assert(ret >= 0);

    ret = pipe(to_client_);
    assert(ret >= 0);
}

BareServer::~BareServer()
{
    close(to_server_[0]);
    close(to_server_[1]);
    close(to_client_[0]);
    close(to_client_[1]);
}

void BareServer::Start()
{
    thread_ = std::thread{thread_entry, to_server_[0], to_client_[1]};
    std::cout << "thread started !!!" << std::endl;
}

void BareServer::Wait() noexcept { thread_.join(); }
// @@@ sample end

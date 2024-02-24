#include <unistd.h>

#include <cstdint>
#include <iostream>

#include "wrapped_server.h"

// @@@ sample begin 0:0

namespace {
enum class Cmd {
    // @@@ ignore begin
    SayHello,
    SayGoodbye,
    Shutdown,
    // @@@ ignore end
};

struct Packet {
    Cmd cmd;
};
}  // namespace

// 以下、bare_server_wrapper.cppのコードとほぼ同じであるため省略。

// @@@ ignore begin
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

void send_message(int wfd, enum Cmd cmd) noexcept
{
    auto packet = Packet{cmd};

    write(wfd, &packet, sizeof(packet));
}
}  // namespace

WrappedServer::WrappedServer() noexcept : to_server_{-1, -1}, to_client_{-1, -1}, thread_{}
{
    pipe(to_server_);
    pipe(to_client_);
}

WrappedServer::~WrappedServer()
{
    close(to_server_[0]);
    close(to_server_[1]);
    close(to_client_[0]);
    close(to_client_[1]);
}

void WrappedServer::Start() { thread_ = std::thread{thread_entry, to_server_[0], to_client_[1]}; }

std::string WrappedServer::say_hello()
{
    char buffer[30];

    send_message(to_server_[1], Cmd::SayHello);
    read(to_client_[0], buffer, sizeof(buffer));

    return buffer;
}

std::string WrappedServer::say_goodbye()
{
    char buffer[30];

    send_message(to_server_[1], Cmd::SayGoodbye);
    read(to_client_[0], buffer, sizeof(buffer));

    return buffer;
}

void WrappedServer::Shutdown() noexcept
{
    send_message(to_server_[1], Cmd::Shutdown);

    thread_.join();
}
// @@@ ignore end
// @@@ sample end

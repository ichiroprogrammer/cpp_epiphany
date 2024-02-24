#pragma once

#include <thread>

// @@@ sample begin 0:0

enum class Cmd {
    SayHello,
    SayGoodbye,
    Shutdown,
};

struct Packet {
    Cmd cmd;
};

class BareServer final {
public:
    BareServer() noexcept;
    ~BareServer();
    int GetPipeW() const noexcept  // クライアントのwrite用
    {
        return to_server_[1];
    }

    int GetPipeR() const noexcept  // クライアントのread用
    {
        return to_client_[0];
    }

    void Start();
    void Wait() noexcept;

private:
    int         to_server_[2];  // サーバへの通信用
    int         to_client_[2];  // クライアントへの通信用
    std::thread thread_;
};
// @@@ sample end

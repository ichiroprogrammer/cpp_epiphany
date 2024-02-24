#pragma once

#include <thread>

// @@@ sample begin 0:0

class WrappedServer {
public:
    WrappedServer() noexcept;
    virtual ~WrappedServer();

    void        Start();
    std::string SayHello() { return say_hello(); }
    std::string SayGoodbye() { return say_goodbye(); }
    void        Shutdown() noexcept;

protected:
    virtual std::string say_hello();    // 後で拡張するためにvirtual
    virtual std::string say_goodbye();  // 同上

private:
    int         to_server_[2];
    int         to_client_[2];
    std::thread thread_;
};
// @@@ sample end

#pragma once
#include <thread>

#include "dip_pipe.h"

namespace ClientNG {
class Client;
}

// @@@ sample begin 0:0

namespace ServerNG {
class Server {
public:
    Server();
    void RequireStringAsync(ClientNG ::Client& client) noexcept;
    // @@@ ignore begin
    ~Server();

private:
    void stop() noexcept;

    Pipe        pipe_;
    std::thread thread_{};
    // @@@ ignore end
};
}  // namespace ServerNG
// @@@ sample end

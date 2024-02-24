#pragma once
#include <cassert>
#include <cstdint>
#include <memory>
#include <string>

#include "dip_pipe.h"
#include "dip_server_ok.h"

// @@@ sample begin 0:0

namespace ClientOK {
class Client : public ServerOK::ClientIF {
public:
    explicit Client(ServerOK::Server& server) noexcept : ClientIF{}, server_{server}, pipe_{} {}

    std::string GetString(uint32_t num);

    // @@@ ignore begin
private:
    virtual void done(std::string* str) noexcept override
    {
        auto const ret = pipe_.Write(&str, sizeof(str));
        assert(ret == sizeof(str));
    }

    std::unique_ptr<std::string> wait_done();

    ServerOK::Server& server_;
    Pipe              pipe_;
    // @@@ ignore end
};
}  // namespace ClientOK
// @@@ sample end

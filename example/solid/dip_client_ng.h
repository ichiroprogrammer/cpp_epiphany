#pragma once
#include <cassert>
#include <cstdint>
#include <memory>
#include <string>

#include "dip_pipe.h"
#include "dip_server_ng.h"

// @@@ sample begin 0:0

namespace ClientNG {
class Client {
public:
    explicit Client(ServerNG::Server& server) noexcept : server_{server}, pipe_{}, num_{0} {}

    std::string GetString(uint32_t num);

    void Done(std::string* str) noexcept  // サーバーからクライアントへのコマンド終了通知
    {
        auto const ret = pipe_.Write(&str, sizeof(str));
        assert(ret == sizeof(str));
    }

    // @@@ ignore begin
    uint32_t GetNum() const noexcept { return num_; }

private:
    void                         set_num(uint32_t num) noexcept { num_ = num; }
    std::unique_ptr<std::string> wait_done();

    ServerNG::Server& server_;
    Pipe              pipe_;
    uint32_t          num_;
    // @@@ ignore end
};
}  // namespace ClientNG
// @@@ sample end

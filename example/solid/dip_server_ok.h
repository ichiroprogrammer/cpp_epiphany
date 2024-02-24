#pragma once
#include <string>
#include <thread>

#include "dip_pipe.h"

// @@@ sample begin 0:0

namespace ServerOK {
class ClientIF {
public:
    ClientIF() noexcept : num_{0} {}
    void Done(std::string* str) { done(str); }  // サーバーからクライアントへのコマンド終了通知
    // @@@ ignore begin
    uint32_t GetNum() const noexcept { return num_; }
    virtual ~ClientIF() = default;

protected:
    void SetNum(uint32_t num) noexcept { num_ = num; }

    // @@@ ignore end
private:
    virtual void done(std::string* str) = 0;
    // @@@ ignore begin
    uint32_t num_;
    // @@@ ignore end
};

class Server {
public:
    Server();
    void RequireStringAsync(ClientIF& client) noexcept;
    // @@@ ignore begin
    ~Server();

private:
    void stop() noexcept;

    Pipe        pipe_;
    std::thread thread_;
    // @@@ ignore end
};
}  // namespace ServerOK
// @@@ sample end

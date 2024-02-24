#pragma once

#include <memory>
#include <string>

// @@@ sample begin 0:0

enum class Cmd;  // C++11からenumは前方宣言できる。
class BareServer;

class BareServerWrapper final {
public:
    BareServerWrapper();

    void        Start();
    std::string SayHello();
    std::string SayGoodbye();
    void        Shutdown() noexcept;

private:
    void                        send_message(enum Cmd cmd) noexcept;
    std::unique_ptr<BareServer> bare_server_;
};
// @@@ sample end

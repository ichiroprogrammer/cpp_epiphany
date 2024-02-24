#pragma once

#include <thread>

#include "wrapped_server.h"

// @@@ sample begin 0:0

class WrappedServerProxy final : public WrappedServer {
public:
    WrappedServerProxy() = default;

private:
    std::string         hello_cashe_{};
    virtual std::string say_hello() override;
    virtual std::string say_goodbye() override;
};
// @@@ sample end

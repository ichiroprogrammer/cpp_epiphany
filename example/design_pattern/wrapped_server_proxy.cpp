#include <unistd.h>

#include <iostream>

#include "wrapped_server_proxy.h"

// @@@ sample begin 0:0

std::string WrappedServerProxy::say_hello()
{
    if (hello_cashe_.size() == 0) {
        hello_cashe_ = WrappedServer::say_hello();  // キャッシュとし保存
    }

    return hello_cashe_;
}

std::string WrappedServerProxy::say_goodbye()
{
    hello_cashe_ = std::string{};  // helloキャッシュをクリア

    return WrappedServer::say_goodbye();
}
// @@@ sample end

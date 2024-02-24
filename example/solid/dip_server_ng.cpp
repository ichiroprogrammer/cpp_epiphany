#include <cassert>

#include "dip_client_ng.h"
#include "dip_server_ng.h"

// @@@ sample begin 0:0

namespace ServerNG {
namespace {
void dispatch(ClientNG::Client& client)  // コマンドのディスパッチ
{
    switch (client.GetNum()) {
    case 1:
        client.Done(new std::string{"hello"});
        break;
    case 2:
        client.Done(new std::string{"good bye"});
        break;
    // @@@ ignore begin
    case 3:
    default:
        client.Done(new std::string{"unknown"});
        break;
        // @@@ ignore end
    }
}

void thread_entry(Pipe& pipe)  // サーバーのスレッド関数
{
    for (;;) {
        ClientNG::Client* client{nullptr};
        auto const        ret = pipe.Read(&client, sizeof(client));
        assert(ret == sizeof(client));

        if (client == nullptr) {  // nullptr受信でサーバー終了
            break;
        }

        dispatch(*client);
    }
}
}  // namespace
// @@@ ignore begin

Server::Server() : pipe_{}, thread_{thread_entry, std::ref(pipe_)} {}

Server::~Server() { stop(); }
// @@@ ignore end

void Server::RequireStringAsync(ClientNG::Client& client) noexcept
{
    void const* const buff{&client};

    auto ret = pipe_.Write(&buff, sizeof(buff));
    assert(ret == sizeof(&client));
}
// @@@ ignore begin

void Server::stop() noexcept  // サーバー終了用関数
{
    void const* const buff{nullptr};

    auto const ret = pipe_.Write(&buff, sizeof(buff));
    assert(ret == sizeof(buff));

    thread_.join();
}
// @@@ ignore end
}  // namespace ServerNG
// @@@ sample end

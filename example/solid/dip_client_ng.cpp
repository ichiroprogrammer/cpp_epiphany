#include "dip_client_ng.h"

// @@@ sample begin 0:0

namespace ClientNG {
std::string Client::GetString(uint32_t num)
{
    set_num(num);
    server_.RequireStringAsync(*this);

    return *wait_done();  // 非同期通知待ち
}

std::unique_ptr<std::string> Client::wait_done()
{
    std::string* str{nullptr};
    auto const   ret = pipe_.Read(&str, sizeof(str));
    assert(ret == sizeof(str));

    return std::unique_ptr<std::string>{str};
}
}  // namespace ClientNG
// @@@ sample end

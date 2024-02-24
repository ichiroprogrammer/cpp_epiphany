#include <unistd.h>

#include <cstdint>

#include "bare_server.h"
#include "bare_server_wrapper.h"

// @@@ sample begin 0:0

BareServerWrapper::BareServerWrapper() : bare_server_{std::make_unique<BareServer>()} {}

void BareServerWrapper::Start() { bare_server_->Start(); }

void BareServerWrapper::send_message(enum Cmd cmd) noexcept
{
    auto packet = Packet{cmd};

    write(bare_server_->GetPipeW(), &packet, sizeof(packet));
}

std::string BareServerWrapper::SayHello()
{
    char buffer[30];

    send_message(Cmd::SayHello);
    read(bare_server_->GetPipeR(), buffer, sizeof(buffer));

    return buffer;
}

std::string BareServerWrapper::SayGoodbye()
{
    char buffer[30];

    send_message(Cmd::SayGoodbye);
    read(bare_server_->GetPipeR(), buffer, sizeof(buffer));

    return buffer;
}

void BareServerWrapper::Shutdown() noexcept
{
    send_message(Cmd::Shutdown);

    bare_server_->Wait();
}
// @@@ sample end

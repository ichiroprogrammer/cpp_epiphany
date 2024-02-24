#include <unistd.h>

#include <cassert>

#include "dip_pipe.h"

Pipe::Pipe() noexcept
{
    int pipe_fds[2]{-1, -1};

    auto const ret = pipe(pipe_fds);
    assert(ret >= 0);

    write_fd_ = pipe_fds[1];
    read_fd_  = pipe_fds[0];
}

int Pipe::Write(void const* buffer, int length) noexcept
{
    return write(write_fd_, buffer, length);
}

int Pipe::Read(void* buffer, int length) noexcept { return read(read_fd_, buffer, length); }

Pipe::~Pipe()
{
    close(write_fd_);
    close(read_fd_);
}

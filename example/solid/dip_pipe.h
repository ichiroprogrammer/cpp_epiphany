#pragma once

class Pipe {
public:
    Pipe() noexcept;
    ~Pipe();

    int Write(void const* buffer, int length) noexcept;
    int Read(void* buffer, int length) noexcept;

    Pipe(Pipe const&)            = delete;
    Pipe& operator=(Pipe const&) = delete;

private:
    int write_fd_{-1};
    int read_fd_{-1};
};

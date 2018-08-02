#pragma once

#include "shared.hpp"

namespace Streamlabs {

class Client : public Connection {
public:
    Client(const std::string &path) : Connection(path) {}

    void Connect() {
        if (!WaitNamedPipeA(path_.c_str(), NMPWAIT_WAIT_FOREVER)) {
            throw fmt::windows_error(GetLastError(), "Error while watching a pipe");
        }
        pipe_ = CreateFileA(
            path_.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
            NULL
        );
        if (pipe_ == nullptr || pipe_ == INVALID_HANDLE_VALUE) {
            throw fmt::windows_error(GetLastError(), "Error while opening a pipe");
        }
        stream_ = std::make_unique<asio::windows::stream_handle>(io_service_, pipe_);
    }
};

}
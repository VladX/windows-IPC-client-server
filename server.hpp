#pragma once

#include "shared.hpp"

namespace Streamlabs {

class Server : public Connection {
public:
    Server(const std::string &path) : Connection(path) {}

    void Connect() {
        pipe_ = CreateNamedPipeA(
            path_.c_str(), // name of the pipe
            PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, // 2-way pipe
            PIPE_TYPE_BYTE, // send data as a byte stream
            1,
            0,
            0,
            0,
            nullptr
        );
        if (pipe_ == nullptr || pipe_ == INVALID_HANDLE_VALUE) {
            throw fmt::windows_error(GetLastError(), "Error while creating a pipe");
        }
        if (!ConnectNamedPipe(pipe_, nullptr)) {
            const auto error = GetLastError();
            CloseHandle(pipe_);
            throw fmt::windows_error(error, "Failed to make connection on named pipe");
        }
        stream_ = std::make_unique<asio::windows::stream_handle>(io_service_, pipe_);
    }
};

}
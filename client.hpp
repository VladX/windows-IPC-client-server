#pragma once

#include "shared.hpp"

namespace Streamlabs {

class Client {
private:
    const std::string path_;
    HANDLE pipe_;

public:
    Client(const std::string &path) : path_(path), pipe_(0) {}

    ~Client() {
        CloseHandle(pipe_);
    }

    void Connect() {
        pipe_ = CreateFileA(
            path_.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
        if (pipe_ == nullptr || pipe_ == INVALID_HANDLE_VALUE) {
            throw fmt::windows_error(GetLastError(), "Error while opening a pipe");
        }
    }
};

}
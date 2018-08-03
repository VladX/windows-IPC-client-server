#pragma once

#include "shared.hpp"

namespace Streamlabs {

class Server : public Connection {
private:
    std::vector<std::unique_ptr<StreamableObject>> objects_;

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

    StreamableObject &AddObject(StreamableObjectType type) {
        objects_.push_back(StreamableObject::Create(type));
        return *objects_.back();
    }

    void CallMethod(uint64_t index, const std::string &name) {
        objects_.at(index)->CallMethod(name);
    }

    void SendObject(const std::unique_ptr<StreamableObject> &object) {
        Connection::SendObject(object);
    }

    void SendObject(uint64_t index) {
        SendObject(objects_.at(index));
    }

    void PrintSummary() {
        std::cout << std::endl << "=== SUMMARY ===\nTotal number of objects stored: " << objects_.size() << std::endl;
        for (size_t i = 0; i < objects_.size(); ++i)
            std::cout << "Index = " << i << "; Value = " << (*objects_[i]) << std::endl;
        std::cout << std::endl;
    }
};

}
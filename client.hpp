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

    void CreateObject(const std::string &type_name) {
        SendObject(StreamableObject::Create(type_name));
    }

    void CallMethod(uint64_t index, const std::unique_ptr<StreamableObject> &method) {
        const auto length = method->GetLength();
        std::vector<uint8_t> data(sizeof(uint64_t) + length);
        *reinterpret_cast<uint64_t*>(data.data()) = index;
        method->Serialize(data.data() + sizeof(uint64_t));
        SendEvent({CALL_METHOD, data.size()}, data.data());
    }

    std::unique_ptr<StreamableObject> InspectObject(uint64_t index) {
        SendEvent({INSPECT_OBJECT, sizeof(index)}, &index);
        EventHeader header;
        auto data = WaitEvent(header);
        const uint8_t* ptr = &data[0];
        StreamableObjectType type = *reinterpret_cast<const StreamableObjectType*>(ptr);
        auto object = StreamableObject::Create(type);
        object->Deserialize(ptr + sizeof(type));
        return object;
    }
};

}
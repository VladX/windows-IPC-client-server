#pragma once

#include "streamableobject.hpp"
#include "event.hpp"
#include "argparser.hpp"
#include "asio.hpp"
#include "fmt/include/format.h"
#include <algorithm>

namespace Streamlabs {

class Connection {
protected:
    const std::string path_;
    HANDLE pipe_;
    asio::io_service io_service_;
    std::unique_ptr<asio::windows::stream_handle> stream_;

public:
    Connection(const std::string &path) : path_(path), pipe_(0) {}

    ~Connection() { CloseHandle(pipe_); }

    void Connect() {}

    void SendEvent(const EventHeader &header, const void* data = nullptr) {
        asio::write(*stream_, asio::buffer(&header, sizeof(header)));
        if (header.length > 0)
            asio::write(*stream_, asio::buffer(data, header.length));
    }

    std::unique_ptr<const uint8_t[]> WaitEvent(EventHeader &header) {
        asio::read(*stream_, asio::buffer(&header, sizeof(header)));
        uint8_t* data = nullptr;
        if (header.length > 0) {
            data = new uint8_t[header.length];
            asio::read(*stream_, asio::buffer(data, header.length));
        }
        return std::unique_ptr<const uint8_t[]>(data);
    }

    void SendObject(const std::unique_ptr<StreamableObject> &object) {
        const StreamableObjectType type = object->GetType();
        const auto length = object->GetLength();
        std::vector<uint8_t> data(sizeof(type) + length);
        *reinterpret_cast<StreamableObjectType*>(data.data()) = type;
        object->Serialize(data.data() + sizeof(type));
        SendEvent({SEND_OBJECT, data.size()}, data.data());
    }
};

}
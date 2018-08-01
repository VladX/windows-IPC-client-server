#pragma once

#include "streamableobject.hpp"
#include <cstring>

class StreamableString : public StreamableObject {
private:
    std::string value_;

public:
    StreamableString() : StreamableObject(STRING) {}

    void Serialize(uint8_t* data) const override {
        *reinterpret_cast<size_t*>(data) = value_.length();
        memcpy(data + sizeof(size_t), value_.data(), value_.length());
    }

    void Deserialize(const uint8_t* data) override {
        const size_t length = *reinterpret_cast<const size_t*>(data);
        value_.assign(reinterpret_cast<const char*>(data) + sizeof(size_t), length);
    }

    size_t GetLength() const override {
        return value_.length() + sizeof(size_t);
    }

    std::ostream& Print(std::ostream& stream) const override {
        return stream << value_;
    }

    std::istream& Scan(std::istream& stream) override {
        return stream >> value_;
    }
};
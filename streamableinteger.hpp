#pragma once

#include "streamableobject.hpp"

namespace Streamlabs {

class StreamableInteger : public StreamableObject {
private:
    int64_t value_;

public:
    StreamableInteger() : StreamableObject(INTEGER), value_(0) {}

    void Serialize(uint8_t* data) const override {
        *reinterpret_cast<decltype(value_)*>(data) = value_;
    }

    void Deserialize(const uint8_t* data) override {
        value_ = *reinterpret_cast<const decltype(value_)*>(data);
    }

    size_t GetLength() const override {
        return sizeof(value_);
    }

    std::ostream& Print(std::ostream& stream) const override {
        return stream << value_;
    }

    std::istream& Scan(std::istream& stream) override {
        return stream >> value_;
    }
};

}
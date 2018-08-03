#pragma once

#include "streamableobject.hpp"
#include <cstring>
#include <cstdlib>

namespace Streamlabs {

class StreamableVector : public StreamableObject {
private:
    std::vector<int64_t> value_;

public:
    StreamableVector() : StreamableObject(VECTOR) {
        RegisterMethod("PopBack", &StreamableVector::PopBack);
        RegisterMethod("PushBackRandom", &StreamableVector::PushBackRandom);
    }

    void Serialize(uint8_t* data) const override {
        *reinterpret_cast<size_t*>(data) = value_.size();
        memcpy(data + sizeof(size_t), value_.data(), value_.size() * sizeof(decltype(value_)::value_type));
    }

    void Deserialize(const uint8_t* data) override {
        const size_t size = *reinterpret_cast<const size_t*>(data);
        const auto ptr = reinterpret_cast<const decltype(value_)::value_type*>(data + sizeof(size_t));
        value_.assign(ptr, ptr + size);
    }

    size_t GetLength() const override {
        return value_.size() * sizeof(decltype(value_)::value_type) + sizeof(size_t);
    }

    std::ostream& Print(std::ostream& stream) const override {
        stream << '[';
        if (value_.size() > 0)
            stream << value_[0];
        for (size_t i = 1; i < value_.size(); ++i)
            stream << ',' << value_[i];
        return stream << ']';
    }

    std::istream& Scan(std::istream& stream) override {
        size_t size;
        stream >> size;
        value_.resize(size);
        for (size_t i = 0; i < size; ++i)
            stream >> value_[i];
        return stream;
    }

    void PopBack() {
        if (value_.empty())
            throw std::out_of_range("Vector is empty");
        value_.pop_back();
    }

    void PushBackRandom() {
        value_.push_back(rand());
    }
};

}
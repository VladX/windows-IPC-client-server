#pragma once

#include <cstdint>
#include <string>
#include <ostream>
#include <istream>

class IStreamable {
public:
    // Serialize to a byte sequence which could be sent through a byte-stream
    virtual void Serialize(uint8_t* data) const = 0;

    // Convert back from bytes to object
    virtual void Deserialize(const uint8_t* data) = 0;

    // Get the length of a serialized byte-reprezentation
    virtual size_t GetLength() const = 0;

    // Print human-readable reprezentation of the object
    virtual std::ostream& Print(std::ostream& stream) const = 0;

    // Scan human-readable reprezentation of the object
    virtual std::istream& Scan(std::istream& stream) = 0;

    // Helper overload for STL streams
    friend std::ostream& operator<<(std::ostream& stream, const IStreamable& streamable) {
        return streamable.Print(stream);
    }

    // Helper overload for STL streams
    friend std::istream& operator>>(std::istream& stream, IStreamable& streamable) {
        return streamable.Scan(stream);
    }
};
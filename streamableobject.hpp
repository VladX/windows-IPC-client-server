#pragma once

#include "streamable.hpp"
#include <memory>
#include <string>
#include <map>
#include <vector>

enum StreamableObjectType {
    INTEGER = 0,
    FLOAT,
    STRING,
    VECTOR,
    kNumberOfObjects,
};

extern const char* const kObjectNames[kNumberOfObjects]; // Object type names as strings

class StreamableObjectSchema {
private:
    typedef void(*MethodPointer)(void*);

    std::vector<std::string> method_names_;
    std::map<std::string, MethodPointer> methods;

    void RegisterMethod(const std::string &name, MethodPointer method) {
        method_names_.push_back(name);
        methods[name] = method;
    }

    void CallMethod(const std::string &name, void* object) const {
        methods.at(name)(object);
    }

public:
    const std::vector<std::string>& GetMethodNames() const { return method_names_; }

    friend class StreamableObject;
};

class StreamableObject : public IStreamable {
protected:
    StreamableObjectType type_;
    StreamableObjectSchema schema_;

    // Register a method with the specified name in the schema which could be called later
    template<typename T>
    void RegisterMethod(const std::string &name, void (T::* method)()) {
        const void* ptr = *reinterpret_cast<const void**>(&method);
        schema_.RegisterMethod(name, reinterpret_cast<StreamableObjectSchema::MethodPointer>(ptr));
    }

public:
    StreamableObject(StreamableObjectType type) : type_(type) {}

    // Get the type of the underlying object
    StreamableObjectType GetType() const { return type_; }

    // Get the schema of the underlying object (methods, attributes, etc...)
    const StreamableObjectSchema& GetSchema() const { return schema_; }

    // Call a method with the specified name
    void CallMethod(const std::string &name) {
        schema_.CallMethod(name, this);
    }

    // Factory method to create an empty object with the given type
    static std::unique_ptr<StreamableObject> Create(StreamableObjectType type);
};
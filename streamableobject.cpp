#include "streamableobject.hpp"
#include "streamableinteger.hpp"
#include "streamablefloat.hpp"
#include "streamablestring.hpp"
#include "streamablevector.hpp"

namespace Streamlabs {

const std::vector<std::string> kObjectNames = {"integer", "float", "string", "vector"};

std::unique_ptr<StreamableObject> StreamableObject::Create(StreamableObjectType type) {
    switch (type) {
        case INTEGER:
            return std::make_unique<StreamableInteger>();
        case FLOAT:
            return std::make_unique<StreamableFloat>();
        case STRING:
            return std::make_unique<StreamableString>();
        case VECTOR:
            return std::make_unique<StreamableVector>();
    }
    throw std::logic_error("Unknown object type");
}

std::unique_ptr<StreamableObject> StreamableObject::Create(const std::string &type_name) {
    const auto type = std::find(kObjectNames.begin(), kObjectNames.end(), type_name) - kObjectNames.begin();
    return Create(static_cast<StreamableObjectType>(type));
}

}
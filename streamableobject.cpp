#include "streamableobject.hpp"
#include "streamableinteger.hpp"
#include "streamablefloat.hpp"
#include "streamablestring.hpp"
#include "streamablevector.hpp"

namespace Streamlabs {

const char* const kObjectNames[kNumberOfObjects] = {"Integer", "Float", "String", "Vector"};

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
    return std::unique_ptr<StreamableObject>{};
}

}
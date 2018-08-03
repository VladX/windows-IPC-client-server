#pragma once

#include <cstdint>

namespace Streamlabs {

enum EventType {
    SEND_OBJECT = 1,
    CALL_METHOD,
    INSPECT_OBJECT,
};

struct EventHeader {
    EventType type;
    uint64_t length;
};

};
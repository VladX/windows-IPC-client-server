#pragma once

#include <cstdint>

namespace Streamlabs {

enum EventType {
    SEND_OBJECT = 1,
    CREATE_OBJECT,
    REQUEST_SUMMARY,
};

struct EventHeader {
    EventType type;
    uint64_t length;
};

};
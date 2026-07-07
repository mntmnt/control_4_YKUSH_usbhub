#pragma once
#include <cstdint>
#include <stdexcept>
#include <vector>

namespace usbswitch::details::lowlevel {
using HidHandler=void*;
using Response=std::vector<uint8_t>;
using Bytes = std::vector<uint8_t>;
}

namespace usbswitch::details {

enum class Port : uint8_t {
    Port1 = 1,
    Port2 = 2,
    Port3 = 3
};


inline bool isValidPort(int port) {
    return static_cast<int>(Port::Port1) <= port && port <= static_cast<int>(Port::Port3);
}


inline Port portFromInt(int port) {
    if ( isValidPort(port) ) {
        return static_cast<Port>(port);
    }
    throw std::invalid_argument("Invalid port number");
}


}

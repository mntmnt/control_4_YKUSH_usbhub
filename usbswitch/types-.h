#pragma once
#include <cstdint>
#include <vector>

namespace usbswitch::details::lowlevel {
using HidHandler=void*;
using Response=std::vector<uint8_t>;
using Bytes = std::vector<uint8_t>;
}

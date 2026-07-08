#pragma once


namespace usbswitch::details::lowlevel {
enum HIDValue {
    VID=0x04D8,
    PID=0xF2F7,

    // VID=0x239A, //9114,
    // PID=0xCAFE,
};
constexpr int ReportSize = 6;
}

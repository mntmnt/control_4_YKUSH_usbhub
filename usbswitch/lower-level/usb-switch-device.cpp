#include "usb-switch-device.h"
#include "constants.h"

#include <QDebug>
#include <QThreadPool>
#include <QScopeGuard>

#include <QRandomGenerator>
#include <chrono>

#include "hidapi.h"

using namespace std::chrono_literals;
using usbswitch::details::lowlevel::Bytes;

namespace usbswitch::details::lowlevel {

namespace {
using CommandCode = uint8_t;
[[maybe_unused]] const auto RGSTRDR = qRegisterMetaType<usbswitch::details::lowlevel::Response>("usbswitch::details::lowlevel::Response");
[[maybe_unused]] const auto RGSTRDP = qRegisterMetaType<usbswitch::details::Port>("usbswitch::details::Port");
[[maybe_unused]] const auto RGSTRDS = qRegisterMetaType<usbswitch::details::PortState>("usbswitch::details::PortState");

constexpr std::chrono::milliseconds ReadTimeout = 100ms;
constexpr int USBReportIdSize = 1;
constexpr unsigned FatalTimeoutSequenceCount = 3;

enum ToggleNibble {
    PowerUp   = 0x10,
    PowerDown = 0x00,
};

enum RequestPosIndeces {
    HIDReportReservedIndex  = 0,
    YKUPStartIndex          = 1,
    YKUPCommandIndex        = YKUPStartIndex,
    YKUPCommandRepeatIndex  = YKUPCommandIndex + 1,
};

enum ResponsePosIndeces {
    SuccessIndex = 0,
    CommandIndex = 1,
};

enum ExpectedValues {
    SuccessValue = 0x01,
};


std::pair<Bytes,CommandCode> createTogglePortRequest(Port port, PortState state) {
    const uint8_t toggleNibble = state == PortState::On ? PowerUp : PowerDown;
    const uint8_t portNibble = static_cast<uint8_t>(port) & 0x0Fu;
    const CommandCode command    = toggleNibble | portNibble;

    Bytes bytes(USBReportIdSize + ReportSize, 0x00);
    bytes[YKUPCommandIndex]       = command;
    bytes[YKUPCommandRepeatIndex] = command;

    return {bytes, command};
}


QString inspectHex(const Bytes & bytes) {
    QStringList list;
    for ( auto byte : bytes ) {
        list.append( QStringLiteral("0x") + QString::number(static_cast<unsigned>(byte), 16) );
    }
    return '{' + list.join(", ") + '}';
}
}


struct UsbSwitchDevice::DownstreamPortArg {
    Port port;
    PortState on;
    CommandCode command;
};


UsbSwitchDevice::UsbSwitchDevice(HidHandler handler, QString info, QObject *parent):
    QObject{parent},
    handler(handler),
    deviceInfo(info) {
}


UsbSwitchDevice::~UsbSwitchDevice() {
    if ( handler ) {
        qDebug() << "[hidtester] closing connection: " << deviceInfo;
        hid_close((hid_device*)handler);
        handler = nullptr;
    }
}


QString UsbSwitchDevice::details() const {
    return deviceInfo;
}


void UsbSwitchDevice::togglePort(Port port, PortState state) {
    const auto && [request, command] = createTogglePortRequest(port, state);
    const DownstreamPortArg downstreamPort{port, state, command};

    qDebug() << "[hidtester] write " << inspectHex(request);

    auto writebytes = hid_write((hid_device*)handler, request.data(), request.size());
    const bool gone = writebytes < 0;
    if ( gone ) {
        qCritical() << "[hidtester][error] device gone #" << writebytes << ' ' << QString::fromWCharArray(hid_error((hid_device*)handler));

        emit disconnected();
    } else {
        // Q_ASSERT( writebytes == usbswitch::ReportSize+1 ); // Note, unfortunately it's not true because, at least on windows it expands it to the report's size
        readStatusBack(downstreamPort);
    }
}


void UsbSwitchDevice::readStatusBack(const DownstreamPortArg & downstreamPortArg) {
    Response response(ReportSize, 0x00);

    const auto readbytes = hid_read_timeout((hid_device*)handler, response.data(), response.size(), static_cast<int>(ReadTimeout.count()));
    qDebug() << "[hidtester] READ: " << readbytes << response << inspectHex(response);

    const bool gone = readbytes < 0;
    const bool received = readbytes > 0;
    if ( gone ) {
        qCritical() << "[hidtester][error] device gone #" << readbytes
                    << " error: "      << QString::fromWCharArray(hid_error((hid_device*)handler))
                    << " read-error: " << QString::fromWCharArray(hid_read_error((hid_device*)handler));

        emit disconnected();
    } else if ( received ) {
        Q_ASSERT_X( readbytes == ReportSize, __func__, "It should not return different size" );
        timeoutErrorCount = 0;

        processStatusResponse(downstreamPortArg, response);
    } else {
        qCritical() << "[hidtester][error] device timeout #" << readbytes
                    << " error: "      << QString::fromWCharArray(hid_error((hid_device*)handler))
                    << " read-error: " << QString::fromWCharArray(hid_read_error((hid_device*)handler));
        emit error(tr("Timeout waiting for device response"));
        if ( timeoutErrorCount++ > FatalTimeoutSequenceCount ) {
            emit fatalError(tr("Too many timeouts waiting for device response"));
        }
    }
}


void UsbSwitchDevice::processStatusResponse(const DownstreamPortArg & downstreamPortArg, const Response & response) {
    const bool success = response[SuccessIndex] == SuccessValue;

    if ( success ) {
        if ( response[CommandIndex] != downstreamPortArg.command ) {
            emit error(tr("Unexpected command echo. Must be %1h: %2").arg(QString::number(downstreamPortArg.command, 16), inspectHex(response)));
        } else {
            emit statusAppliedSuccessfully(downstreamPortArg.port, downstreamPortArg.on, "Bytes: " + inspectHex(response));
        }
    } else {
        emit error(tr("Unexpected 1st byte. Must be %1h: %2").arg(SuccessValue, 0, 16).arg(inspectHex(response)));
    }
}

}

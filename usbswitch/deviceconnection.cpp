#include "deviceconnection.h"
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
const auto RGSTRD = qRegisterMetaType<usbswitch::details::lowlevel::Response>("usbswitch::details::lowlevel::Response");
constexpr std::chrono::milliseconds ReadTimeout = 100ms;
constexpr int USBReportIdSize = 1;

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


std::pair<Bytes,CommandCode> createTogglePortRequest(int port, bool on) {
    Q_ASSERT( 1 <= port && port <= 3 );

    const uint8_t toggleNibble = on ? PowerUp : PowerDown;
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
        list.append( QString::number(static_cast<unsigned>(byte), 16) );
    }
    return '{' + list.join(", ") + '}';
}
}


struct DeviceConnection::DownstreamPortArg {
    int port;
    bool on;
    CommandCode command;
};


DeviceConnection::DeviceConnection(HidHandler handler, QString info, QObject *parent):
    QObject{parent},
    handler(handler),
    deviceInfo(info) {
}


DeviceConnection::~DeviceConnection() {
    if ( handler ) {
        qDebug() << "[hidtester] closing connection: " << deviceInfo;
        hid_close((hid_device*)handler);
        handler = 0x00;
    }
}


QString DeviceConnection::details() const {
    return deviceInfo;
}


void DeviceConnection::togglePort(int port, bool on) {
    const bool validPort = 1 <= port && port <= 3;

    if ( validPort ) {
        const auto && [request, command] = createTogglePortRequest(port, on);
        const DownstreamPortArg downstreamPort{port, on, command};

        qDebug() << "[hidtester] write " << inspectHex(request);

        auto writebytes = hid_write((hid_device*)handler, request.data(), request.size());
        const bool gone = writebytes < 0;
        if ( gone ) {
            qCritical() << "[hidtester][error] device gone" << writebytes << ' ' << hid_error((hid_device*)handler);

            emit disconnected();
        } else {
            // Q_ASSERT( writebytes == usbswitch::ReportSize+1 ); // Note, unfortunately it's not true because, at least on windows it expands it to the report's size
            readStatusBack(downstreamPort);
        }
    } else {
        qCritical() << "[hidtester] logic error! invalid port number" << port;
        Q_ASSERT_X( validPort, __func__, "Logic error! This specific usb switch has 3 ports!" );
    }
}


void DeviceConnection::readStatusBack(const DownstreamPortArg & downstreamPortArg) {
    Response response(ReportSize, 0x00);

    const auto readbytes = hid_read_timeout((hid_device*)handler, response.data(), response.size(), static_cast<int>(ReadTimeout.count()));
    qDebug() << "[hidtester] READ: " << readbytes << response << inspectHex(response);

    const bool gone = readbytes < 0;
    const bool received = readbytes > 0;
    if ( gone ) {
        qCritical() << "[hidtester][error] device gone" << readbytes << hid_error((hid_device*)handler);

        emit disconnected();
    } else if ( received ) {
        Q_ASSERT_X( readbytes == ReportSize, __func__, "It should not return different size" );

        processStatusResponse(downstreamPortArg, response);
    }
}


void DeviceConnection::processStatusResponse(const DownstreamPortArg & downstreamPortArg, const Response & response) {
    const bool success = response[SuccessIndex] == SuccessValue;

    if ( success ) {
        if ( response[CommandIndex] != downstreamPortArg.command ) {
            emit error(tr("Unexpected command echo. Must be %1h: %2").arg(QString::number(downstreamPortArg.command, 16), inspectHex(response)));
        } else {
            emit statusApplied(downstreamPortArg.port, downstreamPortArg.on, "Bytes: " + inspectHex(response));
        }
    } else {
        emit error(tr("Unexpected 1st byte. Must be %1h: %2").arg(SuccessValue, 0, 16).arg(inspectHex(response)));
    }
}

}

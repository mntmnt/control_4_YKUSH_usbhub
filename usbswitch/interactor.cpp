#include "interactor.h"
#include "deviceconnection.h"
#include "usbswitch/types-.h"

#include <QTimer>
#include <QDebug>

using namespace std::chrono_literals;

namespace {
const bool RGSTRD = qRegisterMetaType<usbswitch::details::lowlevel::DeviceConnection*>("usbswitch::details::lowlevel::DeviceConnection*");
}

namespace usbswitch::details {

Interactor::Interactor(QObject *parent):
    QObject{parent} {
}


Interactor::~Interactor() = default;


void Interactor::start(usbswitch::details::lowlevel::DeviceConnection * connection) {
    setDevice(connection);
}


void Interactor::togglePort(int port, bool on) {
    if ( deviceConnection ) {
        if ( isValidPort(port) ) {
            deviceConnection->togglePort(portFromInt(port), portStateFrom(on));
        } else {
            qCritical() << "[interactor] invalid port number #" << port;
        }
    }
}


void Interactor::setDevice(usbswitch::details::lowlevel::DeviceConnection * newDevice) {
    Q_ASSERT( newDevice );
    using usbswitch::details::lowlevel::DeviceConnection;

    deviceConnection = newDevice;
    connect(deviceConnection, &DeviceConnection::disconnected,  this, &Interactor::disconnected);
    connect(deviceConnection, &DeviceConnection::statusApplied, this, &Interactor::parseStatus);
    connect(deviceConnection, &DeviceConnection::error,         this, &Interactor::error);
}


void Interactor::parseStatus(usbswitch::details::Port port, usbswitch::details::PortState state, QString response) {
    emit portStateUpdated(static_cast<int>(port), static_cast<bool>(state), response);
}

}

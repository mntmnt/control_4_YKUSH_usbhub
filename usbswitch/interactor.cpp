#include "interactor.h"
#include "deviceconnection.h"

#include <QTimer>

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
        deviceConnection->togglePort(port, on);
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


void Interactor::parseStatus(int port, bool on, QString response) {
    emit portStateUpdated(port, on, response);
}

}

#include "interactor.h"

#include "lower-level/usb-switch-device.h"
#include "lower-level/types-.h"

#include <QTimer>
#include <QDebug>

namespace {
[[maybe_unused]] const bool RGSTRD = qRegisterMetaType<usbswitch::details::lowlevel::UsbSwitchDevice*>("usbswitch::details::lowlevel::UsbSwitchDevice*");
}

namespace usbswitch::details {

Interactor::Interactor(QObject *parent):
    QObject{parent} {
}


Interactor::~Interactor() = default;


void Interactor::start(usbswitch::details::lowlevel::UsbSwitchDevice * connection) {
    setDevice(connection);
}


void Interactor::togglePort(int port, bool on) {
    if ( switchDevice ) {
        if ( isValidPort(port) ) {
            switchDevice->togglePort(portFromInt(port), portStateFrom(on));
        } else {
            qCritical() << "[interactor] invalid port number #" << port;
        }
    }
}


void Interactor::setDevice(lowlevel::UsbSwitchDevice * newDevice) {
    Q_ASSERT( newDevice );
    using lowlevel::UsbSwitchDevice;

    switchDevice = newDevice;
    connect(switchDevice, &UsbSwitchDevice::disconnected,  this, &Interactor::disconnected);
    connect(switchDevice, &UsbSwitchDevice::statusApplied, this, &Interactor::parseStatus);
    connect(switchDevice, &UsbSwitchDevice::error,         this, &Interactor::error);
}


void Interactor::parseStatus(usbswitch::details::Port port, usbswitch::details::PortState state, QString response) {
    emit portStateUpdated(static_cast<int>(port), static_cast<bool>(state), response);
}

}

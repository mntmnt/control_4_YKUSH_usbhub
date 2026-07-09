#include "device-adapter.h"

#include "lower-level/usb-switch-device.h"
#include "lower-level/types-.h"

#include <QTimer>
#include <QDebug>

namespace {
[[maybe_unused]] const bool RGSTRD = qRegisterMetaType<usbswitch::details::lowlevel::UsbSwitchDevice*>("usbswitch::details::lowlevel::UsbSwitchDevice*");
}

namespace usbswitch::details {

DeviceAdapter::DeviceAdapter(QObject *parent):
    QObject{parent} {
}


DeviceAdapter::~DeviceAdapter() = default;


void DeviceAdapter::start(usbswitch::details::lowlevel::UsbSwitchDevice * connection) {
    Q_ASSERT( connection );
    setDevice(connection);
}


void DeviceAdapter::togglePort(int port, bool on) {
    if ( switchDevice ) {
        if ( isValidPort(port) ) {
            switchDevice->togglePort(portFromInt(port), portStateFrom(on));
        } else {
            qCritical() << "[interactor] invalid port number #" << port;
        }
    }
}


void DeviceAdapter::setDevice(lowlevel::UsbSwitchDevice * newDevice) {
    Q_ASSERT( newDevice );
    using lowlevel::UsbSwitchDevice;

    switchDevice = newDevice;
    connect(switchDevice, &UsbSwitchDevice::disconnected,  this, &DeviceAdapter::disconnected);
    connect(switchDevice, &UsbSwitchDevice::statusAppliedSuccessfully, this, &DeviceAdapter::parseStatus);
    connect(switchDevice, &UsbSwitchDevice::error,         this, &DeviceAdapter::error);
    connect(switchDevice, &UsbSwitchDevice::fatalError,    this, &DeviceAdapter::fatalError);
}


void DeviceAdapter::parseStatus(usbswitch::details::Port port, usbswitch::details::PortState state, QString response) {
    emit portStateUpdated(static_cast<int>(port), static_cast<bool>(state), response);
}

}

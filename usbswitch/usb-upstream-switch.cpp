#include "usb-upstream-switch.h"
#include "connection-manager.h"
#include "device-adapter.h"

#include <QTimer>
#include <QDeadlineTimer>
#include <chrono>

using namespace std::chrono_literals;
namespace usbswitch {

USBUpstreamSwitch::USBUpstreamSwitch(QObject * parent):
    QThread{parent} {

    this->connectionManager = new details::ConnectionManager;
    this->connectionManager->moveToThread(this);
    connect(this, &QThread::finished, connectionManager, &details::ConnectionManager::deleteLater);

    connect(connectionManager, &details::ConnectionManager::connected,  this, &USBUpstreamSwitch::deviceConnected);
    connect(connectionManager, &details::ConnectionManager::connected,  this, &USBUpstreamSwitch::deviceInfoChanged);
    connect(connectionManager, &details::ConnectionManager::twoDevError,this, &USBUpstreamSwitch::errorTooMuch);
    connect(connectionManager, &details::ConnectionManager::failedToConnect,
            this, &USBUpstreamSwitch::errorConnectionFailed);

    deviceAdapter = new details::DeviceAdapter;
    deviceAdapter->moveToThread(this);
    connect(this, &QThread::finished, deviceAdapter, &details::DeviceAdapter::deleteLater);

    connect(connectionManager, &details::ConnectionManager::connected, deviceAdapter,    &details::DeviceAdapter::start);
    connect(deviceAdapter,     &details::DeviceAdapter::disconnected,  connectionManager,&details::ConnectionManager::wait4device);
    connect(deviceAdapter,     &details::DeviceAdapter::disconnected,  this,             &USBUpstreamSwitch::deviceDisconnected);

    connect(deviceAdapter, &details::DeviceAdapter::portStateUpdated, this, &USBUpstreamSwitch::portStateUpdated);
    connect(deviceAdapter, &details::DeviceAdapter::error,            this, &USBUpstreamSwitch::errorReported);

    QTimer::singleShot(100ms, connectionManager, &details::ConnectionManager::wait4device);
    start();
}


USBUpstreamSwitch::~USBUpstreamSwitch() = default;


bool USBUpstreamSwitch::isConnected() const {
    return connected;
}


QString USBUpstreamSwitch::deviceInfo() const {
    return connectionManager->getDeviceInfo();
}


void USBUpstreamSwitch::setConnected(bool on) {
    connected = on;

    emit connectedChanged();
}


void USBUpstreamSwitch::stopService() {
    requestInterruption();
    quit();
    wait(QDeadlineTimer{200ms});
}


void USBUpstreamSwitch::enableUsbPort(int port) {
    QMetaObject::invokeMethod(deviceAdapter, [=,this] { deviceAdapter->togglePort(port, true); }, Qt::QueuedConnection);
}


void USBUpstreamSwitch::disableUsbPort(int port) {
    QMetaObject::invokeMethod(deviceAdapter, [=,this] { deviceAdapter->togglePort(port, false); }, Qt::QueuedConnection);
}

}

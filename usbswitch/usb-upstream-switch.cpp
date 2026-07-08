#include "usb-upstream-switch.h"
#include "connection-manager.h"
#include "interactor.h"

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

    interactor = new  details::Interactor;
    interactor->moveToThread(this);
    connect(this, &QThread::finished, interactor, & details::Interactor::deleteLater);

    connect(connectionManager,  &details::ConnectionManager::connected,     interactor, &details::Interactor::start);
    connect(interactor, &details::Interactor::disconnected, connectionManager,  &details::ConnectionManager::wait4device);
    connect(interactor, &details::Interactor::disconnected, this,       &USBUpstreamSwitch::deviceDisconnected);

    connect(interactor, &details::Interactor::portStateUpdated, this, &USBUpstreamSwitch::portStateUpdated);
    connect(interactor, &details::Interactor::error,            this, &USBUpstreamSwitch::errorReported);

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
    QMetaObject::invokeMethod(interactor, [=,this] { interactor->togglePort(port, true); }, Qt::QueuedConnection);
}


void USBUpstreamSwitch::disableUsbPort(int port) {
    QMetaObject::invokeMethod(interactor, [=,this] { interactor->togglePort(port, false); }, Qt::QueuedConnection);
}

}

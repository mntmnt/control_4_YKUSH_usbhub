#include "usb-upstream-switch.h"
#include "connector.h"
#include "interactor.h"

#include <QTimer>

namespace usbswitch {

USBUpstreamSwitch::USBUpstreamSwitch(QObject * parent):
    QThread{parent} {

    this->connector = new details::Connector;
    this->connector->moveToThread(this);
    connect(this, &QThread::finished, connector, &details::Connector::deleteLater);
    connect(connector, &details::Connector::connected,  this, &USBUpstreamSwitch::deviceConnected);
    connect(connector, &details::Connector::connected,  this, &USBUpstreamSwitch::deviceInfoChanged);
    connect(connector, &details::Connector::twoDevError,this, &USBUpstreamSwitch::errorTooMuch);

    interactor = new  details::Interactor;
    interactor->moveToThread(this);
    connect(this, &QThread::finished, interactor, & details::Interactor::deleteLater);
    connect(connector,  & details::Connector::connected, interactor, &details::Interactor::start);
    connect(interactor, & details::Interactor::disconnected, connector, &details::Connector::wait4device);
    connect(interactor, & details::Interactor::disconnected, this, &USBUpstreamSwitch::deviceDisconnected);

    connect(interactor, & details::Interactor::portStateUpdated, this, &USBUpstreamSwitch::portStateUpdated);
    connect(interactor, & details::Interactor::error,            this, &USBUpstreamSwitch::errorReported);

    QTimer::singleShot(100, connector, &details::Connector::wait4device);
    start();
}


USBUpstreamSwitch::~USBUpstreamSwitch() = default;


bool USBUpstreamSwitch::isConnected() const {
    return connected;
}


QString USBUpstreamSwitch::deviceInfo() const {
    return connector->getDeviceInfo();
}


void USBUpstreamSwitch::setConnected(bool on) {
    connected = on;

    emit connectedChanged();
}


void USBUpstreamSwitch::stopService() {
    requestInterruption();
    quit();
    wait(200);
}


void USBUpstreamSwitch::enableUsbPort(int port) {
    QMetaObject::invokeMethod(interactor, [=,this] { interactor->togglePort(port, true); }, Qt::QueuedConnection);
}


void USBUpstreamSwitch::disableUsbPort(int port) {
    QMetaObject::invokeMethod(interactor, [=,this] { interactor->togglePort(port, false); }, Qt::QueuedConnection);
}

}

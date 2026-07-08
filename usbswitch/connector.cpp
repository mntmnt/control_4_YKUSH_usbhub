#include "connector.h"

#include "lower-level/available-devices.h"
#include "lower-level/usb-switch-device.h"

#include <chrono>
#include <QTimer>
#include <QDebug>

using namespace std::chrono_literals;

using usbswitch::details::lowlevel::AvailableDevices;

namespace usbswitch::details {

Connector::Connector(QObject *parent):
    QObject{parent},
    timer(new QTimer(this)) {
    timer->setSingleShot(true);

    connect(timer, &QTimer::timeout, this, &Connector::updateDeviceList);
}


Connector::~Connector() = default;


QString Connector::getDeviceInfo() const {
    std::lock_guard guard(cachedInfoMutex);

    return cachedInfo;
}


void Connector::wait4device() {
    Q_ASSERT( timer->isSingleShot() );

    setCachedInfo(QString{});

    if ( currentHandler ) {
        delete currentHandler;
        currentHandler = nullptr;
    }
    timer->start(1s);
}


void Connector::updateDeviceList() {
    availableDevices = AvailableDevices::enumerate();

    if ( tryToConnect() == ConnectionStatus::NotOpened ) {
        wait4device();
    }
}


Connector::ConnectionStatus Connector::tryToConnect() {
    if ( availableDevices->size() > 1 ) {
        qCritical() << "[cnct] TWO devices found. Have no idea what to do :-D. I have only one, so I can't test this case.";
        emit twoDevError(availableDevices->list());
    } else if ( availableDevices->size() == 1 ) {
        if ( currentHandler = availableDevices->openDevice(this); currentHandler != nullptr ) {
            timer->stop();
            setCachedInfo(currentHandler->details());
            emit connected(currentHandler);

            return ConnectionStatus::Opened;
        } else {
            qCritical() << "[cnct] Failed to connect";
            emit failedToConnect();
        }
    }
    return ConnectionStatus::NotOpened;
}


void Connector::setCachedInfo(const QString & deviceInfo) {
    std::lock_guard guard(cachedInfoMutex);

    cachedInfo = deviceInfo;
}

}

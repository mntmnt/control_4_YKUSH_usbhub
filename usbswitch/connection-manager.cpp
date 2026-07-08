#include "connection-manager.h"

#include "lower-level/available-devices.h"
#include "lower-level/usb-switch-device.h"

#include <chrono>
#include <QTimer>
#include <QDebug>

using namespace std::chrono_literals;

using usbswitch::details::lowlevel::AvailableDevices;

namespace usbswitch::details {

ConnectionManager::ConnectionManager(QObject *parent):
    QObject{parent},
    timer(new QTimer(this)) {
    timer->setSingleShot(true);

    connect(timer, &QTimer::timeout, this, &ConnectionManager::updateDeviceList);
}


ConnectionManager::~ConnectionManager() = default;


QString ConnectionManager::getDeviceInfo() const {
    std::lock_guard guard(cachedInfoMutex);

    return cachedInfo;
}


void ConnectionManager::wait4device() {
    Q_ASSERT( timer->isSingleShot() );

    setCachedInfo(QString{});

    currentDevice.reset();
    timer->start(1s);
}


void ConnectionManager::updateDeviceList() {
    availableDevices = AvailableDevices::enumerate();

    if ( tryToConnect() == ConnectionStatus::NotOpened ) {
        wait4device();
    }
}


ConnectionManager::ConnectionStatus ConnectionManager::tryToConnect() {
    if ( availableDevices->size() > 1 ) {
        qCritical() << "[cnct] TWO devices found. Have no idea what to do :-D. I have only one, so I can't test this case.";
        emit twoDevError(availableDevices->list());
    } else if ( availableDevices->size() == 1 ) {
        if ( currentDevice = availableDevices->openDevice(); currentDevice != nullptr ) {
            timer->stop();
            setCachedInfo(currentDevice->details());
            emit connected(currentDevice.get());

            return ConnectionStatus::Opened;
        } else {
            qCritical() << "[cnct] Failed to connect";
            emit failedToConnect();
        }
    }
    return ConnectionStatus::NotOpened;
}


void ConnectionManager::setCachedInfo(const QString & deviceInfo) {
    std::lock_guard guard(cachedInfoMutex);

    cachedInfo = deviceInfo;
}

}

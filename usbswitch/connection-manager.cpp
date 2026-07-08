#include "connection-manager.h"

#include "lower-level/available-devices.h"
#include "lower-level/usb-switch-device.h"
#include "exceptions-.h"

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
        processTooManyDevices(availableDevices->list());
    } else if ( availableDevices->size() == 1 ) {
        if ( connectToASingleAvailableDevice() == ConnectionStatus::Opened ) {
            return ConnectionStatus::Opened;
        }
    }
    return ConnectionStatus::NotOpened;
}


void ConnectionManager::setCachedInfo(const QString & deviceInfo) {
    std::lock_guard guard(cachedInfoMutex);

    cachedInfo = deviceInfo;
}


void ConnectionManager::processTooManyDevices(const QStringList & devices) {
    qCritical() << "[cnct] TWO devices found. Have no idea what to do :-D. I have only one, so I can't test this case.";

    emit twoDevError(devices);
}


void ConnectionManager::processFailedToConnect(const QString & errorMessage) {
    qCritical() << "[cnct] Failed to connect: " << errorMessage;

    emit failedToConnect(errorMessage);
}


ConnectionManager::ConnectionStatus ConnectionManager::connectToASingleAvailableDevice() {
    Q_ASSERT(! currentDevice );
    try {
        currentDevice = availableDevices->openDevice();

        timer->stop();
        setCachedInfo(currentDevice->details());

        emit connected(currentDevice.get());
        return ConnectionStatus::Opened;
    } catch ( const FailToOpenException & e ) {
        processFailedToConnect(e.errorMessage());
        return ConnectionStatus::NotOpened;
    } catch ( const std::exception & e ) {
        Q_ASSERT_X( false, __func__, "logic error: unexpected exception");
        processFailedToConnect(QStringLiteral("<logic-error: unexpected exception>: %1").arg(e.what()));
        return ConnectionStatus::NotOpened;
    }
}

}

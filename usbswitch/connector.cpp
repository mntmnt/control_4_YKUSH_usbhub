#include "connector.h"
#include "deviceenumerator.h"
#include "deviceconnection.h"

#include <chrono>
#include <QTimer>
#include <QDebug>

using namespace std::chrono_literals;

using usbswitch::details::lowlevel::DeviceEnumerator;

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
    enumerator = std::make_unique<DeviceEnumerator>();

    if ( processDeviceList() == ListProcessingResult::NotOpened ) {
        wait4device();
    }
}


Connector::ListProcessingResult Connector::processDeviceList() {
    if ( enumerator->size() > 1 ) {
        qCritical() << "[cnct] TWO devices found. Have no idea what to do :-D. I have only one, so I can't test this case.";
        emit twoDevError(enumerator->list());
    } else if ( enumerator->size() == 1 ) {
        if ( currentHandler = enumerator->openDevice(this); currentHandler != nullptr ) {
            timer->stop();
            setCachedInfo(currentHandler->details());
            emit connected(currentHandler);

            return ListProcessingResult::Opened;
        } else {
            qCritical() << "[cnct] Failed to connect";
            emit failedToConnect();
        }
    }
    return ListProcessingResult::NotOpened;
}


void Connector::setCachedInfo(const QString & deviceInfo) {
    std::lock_guard guard(cachedInfoMutex);

    cachedInfo = deviceInfo;
}

}

#include "connector.h"
#include "deviceenumerator.h"
#include "deviceconnection.h"

#include <QTimer>
#include <QDebug>

using usbswitch::details::lowlevel::DeviceEnumerator;

namespace usbswitch::details {

Connector::Connector(QObject *parent):
    QObject{parent},
    timer(new QTimer(this)),
    enumerator(new DeviceEnumerator(this)) {
    timer->setSingleShot(true);

    connect(timer, &QTimer::timeout, enumerator.get(), &DeviceEnumerator::update);
    connect(enumerator.get(), &DeviceEnumerator::updated, this, &Connector::processDeviceList);
}


Connector::~Connector() = default;


usbswitch::details::lowlevel::DeviceConnection *Connector::deviceConnection() const {
    return currentHandler;
}


QString Connector::getDeviceInfo() const {
    return currentHandler ? currentHandler->details() : QString{};
}


void Connector::wait4device() {
    Q_ASSERT( timer->isSingleShot() );

    if ( currentHandler ) {
        delete currentHandler;
        currentHandler = nullptr;
    }
    timer->start(1000);
}


void Connector::processDeviceList() {
    if ( enumerator->size() > 1 ) {
        qCritical() << "[cnct] TWO devices found. Have no idea what to do :-D. I have only one, so I can't test this case.";
        emit twoDevError(enumerator->list());
    } else if ( enumerator->size() == 1 ) {
        if ( currentHandler = enumerator->openDevice(); currentHandler != nullptr ) {
            timer->stop();
            emit connected(currentHandler);
            return;
        } else {
            qCritical() << "[cnct] Failed to connect";
            emit failedToConnect();
        }
    }
    wait4device();
}

}

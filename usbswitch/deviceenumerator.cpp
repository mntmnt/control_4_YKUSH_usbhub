#include "deviceenumerator.h"
#include "deviceconnection.h"
#include "constants.h"

#include "hidapi.h"
#pragma comment (lib, "Setupapi.lib")

namespace usbswitch::details::lowlevel {

namespace {
    QString textFor(const struct hid_device_info * info) {
        return QString("%1 / %2 / %3")
                       .arg( QString::fromWCharArray(info->product_string),
                             QString::fromWCharArray(info->serial_number),
                             QString::number(info->release_number,16)
                             );
    }
}


struct DeviceEnumerator::Pimp {
    struct hid_device_info * info = nullptr;
    QList<QPair<int,QString>> deviceList;

    struct hid_device_info * at(qsizetype searchIndex) {
        auto iter = info;
        qsizetype index = 0;
        while ( iter ) {
            if ( index == searchIndex ) {
                return iter;
            }
            ++index;
            iter = iter->next;
        }
        return nullptr;
    }
};


DeviceEnumerator::DeviceEnumerator(QObject *parent):
    QObject{parent},
    pimp(std::make_unique<Pimp>()) {
}


DeviceEnumerator::~DeviceEnumerator() {
    if ( pimp->info ) {
        hid_free_enumeration(pimp->info);
    }
}


QString DeviceEnumerator::textAt(qsizetype serchee_index) const {
    if ( auto iter = pimp->at(serchee_index) ) {
        return textFor(iter);
    }
    return QString();
}


QStringList DeviceEnumerator::list() const {
    QStringList list;
    list.reserve(10);

    auto iter = pimp->info;
    while ( iter ) {
        list.append(textFor(iter));
        iter = iter->next;
    }
    return list;
}


qsizetype DeviceEnumerator::size() const {
    qsizetype size = 0;
    auto iter = pimp->info;
    while ( iter ) {
        ++size;
        iter = iter->next;
    }
    return size;
}


void DeviceEnumerator::update() {
    if ( pimp->info ) {
        hid_free_enumeration(pimp->info);
    }

    pimp->info = hid_enumerate(VID, PID);
    emit updated();
}


DeviceConnection * DeviceEnumerator::openDevice() {
    constexpr int firstDevIndex = 0;
    if ( auto iter = pimp->at(firstDevIndex) ) {
        if ( auto device = hid_open_path(iter->path) ) {
            return new DeviceConnection((HidHandler)device, textAt(firstDevIndex), this);
        }
    }
    return nullptr;
}

}

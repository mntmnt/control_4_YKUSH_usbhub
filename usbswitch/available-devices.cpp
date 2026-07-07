#include "available-devices.h"
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


struct AvailableDevices::Pimp {
    struct hid_device_info * info = nullptr;

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


AvailableDevices::AvailableDevices():
    pimp(std::make_unique<Pimp>()) {
    update();
}


AvailableDevices::~AvailableDevices() {
    if ( pimp->info ) {
        hid_free_enumeration(pimp->info);
    }
}


QString AvailableDevices::textAt(qsizetype serchee_index) const {
    if ( auto iter = pimp->at(serchee_index) ) {
        return textFor(iter);
    }
    return QString();
}


QStringList AvailableDevices::list() const {
    QStringList list;
    list.reserve(10);

    auto iter = pimp->info;
    while ( iter ) {
        list.append(textFor(iter));
        iter = iter->next;
    }
    return list;
}


qsizetype AvailableDevices::size() const {
    qsizetype size = 0;
    auto iter = pimp->info;
    while ( iter ) {
        ++size;
        iter = iter->next;
    }
    return size;
}


void AvailableDevices::update() {
    Q_ASSERT(! pimp->info );

    pimp->info = hid_enumerate(VID, PID);
}


DeviceConnection * AvailableDevices::openDevice(QObject * parent) {
    constexpr int firstDevIndex = 0;
    if ( auto iter = pimp->at(firstDevIndex) ) {
        if ( auto device = hid_open_path(iter->path) ) {
            return new DeviceConnection((HidHandler)device, textAt(firstDevIndex), parent);
        }
    }
    return nullptr;
}

}

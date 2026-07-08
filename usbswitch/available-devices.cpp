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

    template<class Block>
    void forEachDevice(const struct hid_device_info * info, Block && block) {
        auto iter = info;
        while ( iter ) {
            block(iter);
            iter = iter->next;
        }
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


AvailableDevices::AvailableDevices(AvailableDevices::Private):
    pimp(std::make_unique<Pimp>()) {
    update();
}


AvailableDevices::~AvailableDevices() {
    if ( pimp->info ) {
        hid_free_enumeration(pimp->info);
    }
}


std::unique_ptr<AvailableDevices> AvailableDevices::enumerate() {
    return std::make_unique<AvailableDevices>(Private{});
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

    forEachDevice(pimp->info, [&list](const auto * iter) {
        list.append(textFor(iter));
    });
    return list;
}


qsizetype AvailableDevices::size() const {
    qsizetype size = 0;
    forEachDevice(pimp->info, [&size](const auto * /*iter*/) {
        ++size;
    });
    return size;
}


void AvailableDevices::update() {
    Q_ASSERT(! pimp->info );

    pimp->info = hid_enumerate(VID, PID);
}


UsbSwitchDevice * AvailableDevices::openDevice(QObject * parent) {
    constexpr int firstDevIndex = 0;
    if ( auto iter = pimp->at(firstDevIndex) ) {
        if ( auto device = hid_open_path(iter->path) ) {
            return new UsbSwitchDevice((HidHandler)device, textAt(firstDevIndex), parent);
        }
    }
    return nullptr;
}

}

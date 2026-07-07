#pragma once

#include <QObject>
#include <memory>

namespace usbswitch::details::lowlevel {

class DeviceConnection;

class AvailableDevices final {
public:
    explicit AvailableDevices();
    ~AvailableDevices();

    [[nodiscard]] DeviceConnection * openDevice(QObject * parent);
    [[nodiscard]] qsizetype size() const;
    [[nodiscard]] QStringList list() const;

private:
    void update();
    QString textAt(qsizetype) const;

    struct Pimp;
    const std::unique_ptr<Pimp> pimp;
};

}

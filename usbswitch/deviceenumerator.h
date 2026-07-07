#pragma once

#include <QObject>
#include <memory>

namespace usbswitch::details::lowlevel {

class DeviceConnection;

class DeviceEnumerator {
public:
    explicit DeviceEnumerator();
    ~DeviceEnumerator();

    [[nodiscard]] DeviceConnection * openDevice(QObject * parent);

    [[nodiscard]] qsizetype size() const;

    [[nodiscard]] QStringList list() const;

    void update();

private:
    QString textAt(qsizetype) const;

    struct Pimp;
    const std::unique_ptr<Pimp> pimp;
};

}

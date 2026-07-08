#pragma once

#include <QObject>
#include <memory>

namespace usbswitch::details::lowlevel {

class DeviceConnection;

class AvailableDevices final {
    struct Private {
        explicit Private() = default;
    };

public:

    explicit AvailableDevices(Private);
    ~AvailableDevices();

    [[nodiscard]] static std::unique_ptr<AvailableDevices> enumerate();

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

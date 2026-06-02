#pragma once

#include <QObject>
#include <memory>

namespace usbswitch::details::lowlevel {

class DeviceConnection;

class DeviceEnumerator : public QObject {
    Q_OBJECT

public:
    explicit DeviceEnumerator(QObject *parent = nullptr);
    ~DeviceEnumerator();

    [[nodiscard]] DeviceConnection * openDevice();

    [[nodiscard]] qsizetype size() const;

    [[nodiscard]] QStringList list() const;

signals:
    void updated();

public slots:
    void update();

private:
    QString textAt(qsizetype) const;

    struct Pimp;
    const std::unique_ptr<Pimp> pimp;
};

}

#pragma once

#include <QObject>
#include <QPointer>

#include "lower-level/usb-switch-device.h"

QT_BEGIN_NAMESPACE
class QTimer;
QT_END_NAMESPACE

namespace usbswitch::details {

class DeviceAdapter : public QObject {
    Q_OBJECT

public:
    explicit DeviceAdapter(QObject *parent = nullptr);
    ~DeviceAdapter();

signals:
    void disconnected();
    void portStateUpdated(int, bool, QString);
    void error(QString);
    void fatalError(QString);

public slots:
    void start(usbswitch::details::lowlevel::UsbSwitchDevice *);
    void togglePort(int, bool on);

private slots:
    void parseStatus(usbswitch::details::Port, usbswitch::details::PortState, QString);

private:
    void setDevice(lowlevel::UsbSwitchDevice *);

    QPointer<lowlevel::UsbSwitchDevice> switchDevice { nullptr };

};

}

#pragma once

#include <QObject>

#include "types-.h"

namespace usbswitch::details::lowlevel {

class UsbSwitchDevice : public QObject {
    Q_OBJECT

public:
    struct DownstreamPortArg;

    explicit UsbSwitchDevice(HidHandler, QString info, QObject *parent = nullptr);
    ~UsbSwitchDevice();

    QString details() const;

signals:
    void statusApplied(usbswitch::details::Port, usbswitch::details::PortState, QString);
    void disconnected();

    void error(QString);

public slots:
    void togglePort(usbswitch::details::Port, usbswitch::details::PortState);

private:
    void readStatusBack(const DownstreamPortArg &);
    void processStatusResponse(const DownstreamPortArg &, const Response &);

    HidHandler handler;
    const QString deviceInfo;
};

}

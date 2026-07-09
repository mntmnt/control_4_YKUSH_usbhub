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
    void statusAppliedSuccessfully(usbswitch::details::Port, usbswitch::details::PortState, QString);
    void disconnected();

    void error(QString);
    void fatalError(QString);

public slots:
    void togglePort(usbswitch::details::Port, usbswitch::details::PortState);

private:
    void readStatusBack(const DownstreamPortArg &);
    void processStatusResponse(const DownstreamPortArg &, const Response &);

    HidHandler handler;
    const QString deviceInfo;
    unsigned timeoutErrorCount { 0 };
};

}

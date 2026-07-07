#pragma once

#include <QObject>

#include "usbswitch/types-.h"

namespace usbswitch::details::lowlevel {

class DeviceConnection : public QObject {
    Q_OBJECT

public:
    struct DownstreamPortArg;

    explicit DeviceConnection(HidHandler, QString info, QObject *parent = nullptr);
    ~DeviceConnection();

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

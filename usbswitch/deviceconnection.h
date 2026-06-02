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
    void statusApplied(int port, bool on, QString);
    void disconnected();

    void error(QString);

public slots:
    void togglePort(int port, bool on);

private:
    void readStatusBack(const DownstreamPortArg &);
    void processStatusResponse(const DownstreamPortArg &, const Response &);

    HidHandler handler;
    const QString deviceInfo;
};

}

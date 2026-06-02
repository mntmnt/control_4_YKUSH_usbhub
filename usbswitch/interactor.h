#pragma once

#include <QObject>
#include <QPointer>

#include "deviceconnection.h"

QT_BEGIN_NAMESPACE
class QTimer;
QT_END_NAMESPACE

namespace usbswitch::details {

class Interactor : public QObject {
    Q_OBJECT

public:
    explicit Interactor(QObject *parent = nullptr);
    ~Interactor();

signals:
    void disconnected();
    void portStateUpdated(int, bool, QString);
    void error(QString);

public slots:
    void start(usbswitch::details::lowlevel::DeviceConnection *);
    void togglePort(int, bool on);

private slots:
    void parseStatus(int port, bool on, QString);

private:
    void setDevice(usbswitch::details::lowlevel::DeviceConnection *);

    QPointer<usbswitch::details::lowlevel::DeviceConnection> deviceConnection { nullptr };

};

}

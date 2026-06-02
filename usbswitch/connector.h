#pragma once

#include <QObject>

QT_BEGIN_NAMESPACE
class QTimer;
QT_END_NAMESPACE

namespace usbswitch::details::lowlevel {
class DeviceEnumerator;
class DeviceConnection;
}

namespace usbswitch::details {

class Connector : public QObject {
    Q_OBJECT
public:
    explicit Connector(QObject *parent = nullptr);
    ~Connector();

    usbswitch::details::lowlevel::DeviceConnection * deviceConnection() const;

    QString getDeviceInfo() const;

signals:
    void connected(usbswitch::details::lowlevel::DeviceConnection *);
    void twoDevError(QStringList);
    void failedToConnect();

public slots:
    void wait4device();

private slots:
    void processDeviceList();

private:
    QTimer * timer;
    const std::unique_ptr<usbswitch::details::lowlevel::DeviceEnumerator> enumerator;
    usbswitch::details::lowlevel::DeviceConnection * currentHandler { nullptr };
};

}

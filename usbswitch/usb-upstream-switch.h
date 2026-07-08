#pragma once

#include <QObject>
#include <QThread>
#include <atomic>

QT_BEGIN_NAMESPACE
class QTimer;
QT_END_NAMESPACE

namespace usbswitch::details {
class ConnectionManager;
class DeviceAdapter;
}


namespace usbswitch {

class USBUpstreamSwitch : public QThread {
    Q_OBJECT

    Q_PROPERTY(bool connected READ isConnected WRITE setConnected NOTIFY connectedChanged);
    Q_PROPERTY(QString deviceInfo READ deviceInfo NOTIFY deviceInfoChanged);

public:
    explicit USBUpstreamSwitch(QObject * parent = nullptr);
    ~USBUpstreamSwitch();

    bool isConnected() const;
    QString deviceInfo() const;

signals:
    void connectedChanged();
    void deviceConnected();
    void deviceDisconnected();

    void errorTooMuch(QStringList);
    void deviceInfoChanged();

    void portStateUpdated(int, bool, QString);
    void errorReported(QString);

public slots:
    void setConnected(bool);
    void stopService();

    void enableUsbPort(int);
    void disableUsbPort(int);

private:
    std::atomic<bool> connected { false };

    details::ConnectionManager * connectionManager { nullptr };
    details::DeviceAdapter * deviceAdapter { nullptr };

};

}

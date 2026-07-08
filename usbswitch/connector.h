#pragma once

#include <QObject>
#include <mutex>
#include <memory>

QT_BEGIN_NAMESPACE
class QTimer;
QT_END_NAMESPACE

namespace usbswitch::details::lowlevel {
class AvailableDevices;
class UsbSwitchDevice;
}

namespace usbswitch::details {

class Connector : public QObject {
    Q_OBJECT
public:
    enum class ConnectionStatus { NotOpened, Opened };

    explicit Connector(QObject *parent = nullptr);
    ~Connector();

    QString getDeviceInfo() const;

signals:
    void connected(usbswitch::details::lowlevel::UsbSwitchDevice *);
    void twoDevError(QStringList);
    void failedToConnect();

public slots:
    void wait4device();

private slots:
    void updateDeviceList();

private:
    ConnectionStatus tryToConnect();
    void setCachedInfo(const QString &);

    QTimer * timer;
    std::unique_ptr<lowlevel::AvailableDevices> availableDevices;
    lowlevel::UsbSwitchDevice * currentHandler { nullptr };

    mutable std::mutex cachedInfoMutex;
    QString cachedInfo;
};

}

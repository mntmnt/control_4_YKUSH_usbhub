#pragma once

#include <QObject>
#include <mutex>

QT_BEGIN_NAMESPACE
class QTimer;
QT_END_NAMESPACE

namespace usbswitch::details::lowlevel {
class AvailableDevices;
class DeviceConnection;
}

namespace usbswitch::details {

class Connector : public QObject {
    Q_OBJECT
public:
    enum class ListProcessingResult { NotOpened, Opened };

    explicit Connector(QObject *parent = nullptr);
    ~Connector();

    QString getDeviceInfo() const;

signals:
    void connected(usbswitch::details::lowlevel::DeviceConnection *);
    void twoDevError(QStringList);
    void failedToConnect();

public slots:
    void wait4device();

private slots:
    void updateDeviceList();

private:
    ListProcessingResult processDeviceList();
    void setCachedInfo(const QString &);

    QTimer * timer;
    std::unique_ptr<usbswitch::details::lowlevel::AvailableDevices> enumerator;
    usbswitch::details::lowlevel::DeviceConnection * currentHandler { nullptr };
    mutable std::mutex cachedInfoMutex;
    QString cachedInfo;
};

}

#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QHostAddress>

enum class DeviceStatus {
    OFF = 0,
    ON = 1,
    ERROR = 2
};
class Device : public QObject
{
    Q_OBJECT

public:

    explicit Device(QObject *parent = nullptr);
    Device(const QString& name, const QHostAddress& ip, QObject *parent = nullptr);
    virtual ~Device() = default;

    DeviceStatus getStatus() const { return status; }
    QString getName() const { return name; }
    QHostAddress getDeviceIP() const { return deviceIP; }
    QByteArray getCurrentCommand() const{
        return command;
    }

    void setDeviceName(const QString& name);
    void setStatus(DeviceStatus st);
    void setCommand(const QByteArray& comm);
    void setDeviceIP(const QHostAddress& ip);


    virtual void sendCommand();

protected:
    QString name;
    DeviceStatus status;
    QHostAddress deviceIP;
    QByteArray command;

signals:
    void statusChanged(DeviceStatus newStatus);
    void commandSent(const QByteArray& command);
    void deviceNameChanged(const QString& newName);
    void connectionError(const QString& errorMessage);
    void sendCommandSignal(const QByteArray& command, const QHostAddress& deviceIP);

public slots:
    virtual void updateStatus();
    virtual void handleResponse(const QByteArray& response);
};

#endif // DEVICE_H

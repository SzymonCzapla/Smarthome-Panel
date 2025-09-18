#include "actuator.h"

Actuator::Actuator(QObject *parent)
    : Device{parent}
{
}

Actuator::Actuator(const QString &name, const QHostAddress &ip, QObject *parent)
{
    setDeviceName(name);
    setDeviceIP(ip);
    setStatus(DeviceStatus::OFF);
}

void Actuator::sendCommand()
{
    emit sendCommandSignal(getCurrentCommand(), getDeviceIP());
}

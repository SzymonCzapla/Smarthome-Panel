#include "device.h"

Device::Device(QObject *parent)
    : QObject(parent)
    , status(DeviceStatus::OFF)
{
}

Device::Device(const QString& name, const QHostAddress& ip, QObject *parent)
    : QObject(parent)
    , name(name)
    , status(DeviceStatus::OFF)
    , deviceIP(ip)
{
}

void Device::setDeviceName(const QString& newName)
{
    if (name != newName) {
        name = newName;
        emit deviceNameChanged(name);
    }
}

void Device::setStatus(DeviceStatus st)
{
    if (status != st) {
        status = st;
        emit statusChanged(status);
    }
}

void Device::setCommand(const QByteArray& comm)
{
    command = comm;
}

void Device::setDeviceIP(const QHostAddress& ip)
{
    //validate if really IP
    deviceIP = ip;
}

void Device::sendCommand()
{
    emit sendCommandSignal(getCurrentCommand(), getDeviceIP());
}

void Device::updateStatus()
{

}

void Device::handleResponse(const QByteArray &response)
{

}

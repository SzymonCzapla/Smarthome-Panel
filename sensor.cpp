#include "sensor.h"

Sensor::Sensor(QObject *parent)
    : Device{parent}
{}

void Sensor::sendCommand()
{
    emit sendCommandSignal(getCurrentCommand(), getDeviceIP());
}

void Sensor::handleResponse(const QByteArray &response)
{
    readingValue = response[3]; //example
    //emit signal with readingValue
}



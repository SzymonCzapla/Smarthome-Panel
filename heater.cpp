#include "heater.h"

Heater::Heater(QObject *parent)
    : Actuator{parent}
{
    setDeviceName("Heater");
    setStatus(DeviceStatus::OFF);
    setCommand(QByteArray(1,0));
}
void Heater::toggle()
{
    QByteArray temp_command(1,0); //1Bajt
    if(getStatus()== DeviceStatus::ON){
        temp_command[0] = 0x00;
        setCommand(temp_command);
        sendCommand();
        setStatus(DeviceStatus::OFF);
        emit heaterToggled(false);

    } else{
        temp_command[0] = 0x01;
        setCommand(temp_command);
        sendCommand();
        setStatus(DeviceStatus::ON);
        emit heaterToggled(true);
    }
}

void Heater::sendCommand()
{
    emit sendCommandSignal(getCurrentCommand(), getDeviceIP());
}
void Heater::onUIToggle()
{
    toggle();
}

void Heater::onTurnHeater_ON(){
    setStatus(DeviceStatus::ON);
    emit heaterToggled(true);
    qDebug() << "Heater włączone";
}

void Heater::onTurnHeater_OFF(){
    setStatus(DeviceStatus::OFF);
    emit heaterToggled(false);
    qDebug() << "Heater wyłączone";
}

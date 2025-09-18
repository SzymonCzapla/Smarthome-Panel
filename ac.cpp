#include "ac.h"

AC::AC(QObject *parent)
    : Actuator{parent}
{
    setDeviceName("A/C");
    setStatus(DeviceStatus::OFF);
    setCommand(QByteArray(1,0));
}

void AC::toggle()
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
void AC::sendCommand()
{
    emit sendCommandSignal(getCurrentCommand(), getDeviceIP());
}
void AC::onUIToggle()
{
    toggle();
}

void AC::onTurnAC_ON(){
    setStatus(DeviceStatus::ON);
    emit heaterToggled(false);
    qDebug() << "AC włączone";
}

void AC::onTurnAC_OFF(){
    setStatus(DeviceStatus::OFF);
    emit heaterToggled(false);
    qDebug() << "AC wyłączone";
}

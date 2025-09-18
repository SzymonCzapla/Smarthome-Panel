#include "rgblamp.h"

RGBLamp::RGBLamp(QObject *parent)
    : Lamp{parent}
{
    setCommand(QByteArray(5,0));
    setStatus(DeviceStatus::OFF);
}


void RGBLamp::setBrightness(uint8_t val)
{
    if(0<=val && val <=255){
        m_brightness = val;
    }
}

void RGBLamp::mixColors(uint8_t R, uint8_t G, uint8_t B)
{
    QColor tempColor = QColor(R,G,B);
    if(tempColor.isValid()){
        m_mixedColor = tempColor;
        qDebug() << tempColor.red();
    }
    //else throw an error or something
}

void RGBLamp::chngBrightness()
{
    QByteArray temp_command = getCurrentCommand();
    temp_command[3] = getBrightness();
    setCommand(temp_command);
}

void RGBLamp::chngColor()
{
    QByteArray temp_command = getCurrentCommand();
    temp_command[0] = m_mixedColor.red();
    temp_command[1] = m_mixedColor.green();
    temp_command[2] = m_mixedColor.blue();
    setCommand(temp_command);
}

void RGBLamp::toggle()
{
    QByteArray temp_command = getCurrentCommand();
    if(getStatus()== DeviceStatus::ON){
        temp_command[4] = 0x00;
        setCommand(temp_command);
        sendCommand();
        setStatus(DeviceStatus::OFF);

    } else{
        temp_command[4] = 0x01;
        setCommand(temp_command);
        sendCommand();
        setStatus(DeviceStatus::ON);
    }
}

void RGBLamp::sendCommand()
{
    emit sendCommandSignal(getCurrentCommand(), getDeviceIP());
}

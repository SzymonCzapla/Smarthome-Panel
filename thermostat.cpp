#include "thermostat.h"

Thermostat::Thermostat(QObject *parent)
    : Sensor{parent}
{
    setStatus(DeviceStatus::ON);
    temperatureSetting = 20.0F;
    setDeviceName("Termostat");
    setCommand(QByteArray(3,0));
}


void Thermostat::readValue()
{
    //request response on demand
    if(status == DeviceStatus::ON)
    {
        QByteArray read_command(3,0);
        read_command[0] = 0xFF;
        read_command[1] = 0x00;
        read_command[2] = 0xFF;
        setCommand(read_command);
        sendCommand();
    } else {
        //error window or window with "Plug thermostat to outlet"
    }

}

void Thermostat::setTemperatureSetting(float temp)
{
    if(LOWEST_TEMP <= temp && temp <=HIGHEST_TEMP){
        temperatureSetting = temp;
    } else {
        status = DeviceStatus::ERROR;
    }
    qDebug() << "Thermostat tempreture setting changed!";
    performAction();
}

void Thermostat::setTemperatureReading(float temp)
{
    if(LOWEST_TEMP <= temp && temp <=HIGHEST_TEMP){
        temperatureReading = temp;
    } else {
        status = DeviceStatus::ERROR;
    }
    performAction();
}

void Thermostat::chngTempSetting()
{
    QByteArray temp_command = getCurrentCommand();
    temp_command[0] = 0x02; //write
    temp_command[1] = getTemperatureSetting();
    temp_command[2] = 0x01; //ON
    setCommand(temp_command);
    sendCommand();

    emit temperatureSettingChanged(getTemperatureSetting());
}

void Thermostat::performAction()
{
    if(getTemperatureReading() > getTemperatureSetting()){
        //turn on AC
        emit turnHVACs_OFF();
        emit turnAC_ON();
    }else if(getTemperatureReading() < getTemperatureSetting()){
        //turn on heater
        emit turnHVACs_OFF();
        emit turnHeater_ON();
    }else{
        //turn devices off
        emit turnHVACs_OFF();
    }
}


void Thermostat::sendCommand()
{
    emit sendCommandSignal(getCurrentCommand(), getDeviceIP());
}

void Thermostat::handleResponse(const QByteArray &response)
{
    if(response[3] == 0x0F){
        //ERROR handling
    }else if(response[0] == 0x01){
        setTemperatureReading(response[1]);
    }
    //do something/update deviceInfo in main
}


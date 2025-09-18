#include "hvac.h"

HVAC::HVAC(Thermostat *thermosta,QObject *parent)
    : Device{parent}, thermostat(thermosta)
{
    this->setStatus(DeviceStatus::ON);
}

void HVAC::addAC(AC* ac){
    devicesAC.append(ac);
    qDebug() << ac->getName();
    qDebug() << ac->getDeviceIP();
    connect(thermostat,&Thermostat::turnAC_ON,ac,&AC::onTurnAC_ON);
    connect(thermostat,&Thermostat::turnHVACs_OFF,ac,&AC::onTurnAC_OFF);
}

void HVAC::addHeater(Heater* heater){
    devicesHeater.append(heater);
    qDebug() << heater->getName();
    qDebug() << heater->getDeviceIP();
    connect(thermostat,&Thermostat::turnHeater_ON,heater,&Heater::onTurnHeater_ON);
    connect(thermostat,&Thermostat::turnHVACs_OFF,heater,&Heater::onTurnHeater_OFF);
}

#ifndef HVAC_H
#define HVAC_H

#include <QObject>
#include "device.h"
#include "thermostat.h"
#include "ac.h"
#include "heater.h"

class HVAC : public Device
{
    Q_OBJECT
public:
    explicit HVAC(Thermostat *thermostat,QObject *parent = nullptr);
    void addAC(AC* ac);
    void addHeater(Heater* heater);
    Thermostat *getThermostat(){return thermostat;}
    QList<AC*> getDevicesAC(){ return devicesAC;}
    QList<Heater*> getDevicesHeater(){return devicesHeater;}

private:
    Thermostat* thermostat;
    QList<AC*> devicesAC;
    QList<Heater*> devicesHeater;

signals:
    void turnACs_ON();
    void turnACs_OFF();
    void turnHeaters_ON();
    void turnHeaters_OFF();
};

#endif // HVAC_H

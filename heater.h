#ifndef HEATER_H
#define HEATER_H

#include <QObject>
#include "actuator.h"

class Heater : public Actuator
{
    Q_OBJECT
public:
    explicit Heater(QObject *parent = nullptr);
    void toggle() override;
    void sendCommand() override;
private slots:
    void onUIToggle();
   // void onTurnHeater_ON();
    //implement someohow slot to turn heater on
public slots:
    void onTurnHeater_ON();
    void onTurnHeater_OFF();
signals:
    void heaterToggled(bool isOn);
};

#endif // HEATER_H

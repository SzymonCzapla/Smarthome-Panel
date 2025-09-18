#ifndef LAMP_H
#define LAMP_H

#include "actuator.h"

class Lamp : public Actuator
{
    Q_OBJECT
public:
    explicit Lamp(QObject *parent = nullptr);
    void toggle() override;
    void sendCommand() override;
private slots:
    void onUIToggle();
signals:
    void lampToggled(bool isOn);
};

#endif // LAMP_H

#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "device.h"
class Actuator : public Device
{
    Q_OBJECT

public:
    explicit Actuator(QObject *parent = nullptr);
    Actuator(const QString& name, const QHostAddress& ip, QObject *parent = nullptr);
    virtual void toggle() = 0;
    void sendCommand() override;

};

#endif // ACTUATOR_H

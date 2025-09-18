#ifndef AC_H
#define AC_H

#include <QObject>
#include "actuator.h"

class AC : public Actuator
{
    Q_OBJECT
public:
    explicit AC(QObject *parent = nullptr);
    void toggle() override;
    void sendCommand() override;
private slots:
    void onUIToggle();
    // void onTurnAC_ON();
    //implement someohow slot to turn AC on, unless won't need
public slots:
    void onTurnAC_ON();
    void onTurnAC_OFF();
signals:
    void heaterToggled(bool isOn);
};

#endif // AC_H

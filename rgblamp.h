#ifndef RGBLAMP_H
#define RGBLAMP_H

#include "lamp.h"
#include <QColor>


class RGBLamp : public Lamp
{
    Q_OBJECT
private:
    uint8_t m_brightness;
    QColor m_mixedColor;

public:
    explicit RGBLamp(QObject *parent = nullptr);

    uint8_t getBrightness(){
        return m_brightness;
    }
    QColor getColor(){
        return m_mixedColor;
    }
    void setBrightness(uint8_t val);
    void mixColors(uint8_t R, uint8_t G, uint8_t B);
    void chngBrightness(); //used on signal
    void chngColor();
    void toggle() override;
    void sendCommand() override;

};

#endif // RGBLAMP_H

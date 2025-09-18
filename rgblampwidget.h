#ifndef RGBLAMPWIDGET_H
#define RGBLAMPWIDGET_H

#include <QObject>
#include "devicewidget.h"
#include "rgblamp.h"

namespace Ui {
class RGBLampWidget;
}


class RGBLampWidget : public DeviceWidget
{
    Q_OBJECT
public:
    explicit RGBLampWidget(RGBLamp* lamp, QWidget *parent = nullptr);
    ~RGBLampWidget();
    void updateUI() override;

private:
    RGBLamp* m_lamp() {return qobject_cast<RGBLamp*>(m_device);}
    Ui::RGBLampWidget *ui;
private slots:
    void on_colorPickerButton_clicked();
    void on_brightnessSlider_valueChanged(int value);
    void on_toggleButton_clicked();
};

#endif // RGBLAMPWIDGET_H

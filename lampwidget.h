#ifndef LAMPWIDGET_H
#define LAMPWIDGET_H

#include <QWidget>
#include "devicewidget.h"
#include "lamp.h"

namespace Ui {
class LampWidget;
}

class LampWidget : public DeviceWidget
{
    Q_OBJECT

public:
    explicit LampWidget(Lamp* lamp, QWidget *parent = nullptr);
    ~LampWidget();
    void updateUI() override;

private:
    Ui::LampWidget *ui;
    Lamp* m_lamp() {return qobject_cast<Lamp*>(m_device);}
private slots:

    void on_toggleButton_clicked();
};

#endif // LAMPWIDGET_H
